//
//  trackerKCF.cpp
//  vt
//
//  Created by Yizhuang Zhou on 25/09/2017.
//  Copyright © 2017 Yizhuang Zhou. All rights reserved.
//

#include "trackerKCF.hpp"
#include "recttools.hpp"
#include "label.hpp"
#include "imagesc.hpp"
#include "circshift.hpp"
#include "ffttools.hpp"
#include "fourier_coefficients.hpp"

float subPixelPeak(float left, float center, float right) {
	float divisor = 2 * center - right - left;
	if (divisor == 0)
		return 0;
	return 0.5 * (right - left) / divisor;
}

KCF::KCF(parameter params) : params(params) {}

Size2i KCF::best_suited_feed_size(Size2f input_size, string mode) {
	if (strcmp(mode.data(), "square")) {
		int cellsize = 4;
		int feed_sz_ = (floorf(sqrt(input_size.area())/cellsize/2)*2+1)*cellsize; // round to even number
		return Size2i(feed_sz_, feed_sz_);
	} else if (strcmp(mode.data(), "test")) {
		int cellsize = 4;
		int feed_h_ = (ceilf(input_size.height/cellsize/2)*2+1)*cellsize;
		int feed_w_ = (ceilf(input_size.width/cellsize/2)*2+1)*cellsize;
		return Size2i(feed_h_, feed_w_);
	} else {
		LOG(FATAL) << "No search window mode " << mode;
	}
}

Rect2f KCF::update_sample_window() {
	window.width = window_size.width;
	window.height = window_size.height;
	window.x = center[0] - window.width / 2;
	window.y = center[1] - window.height / 2;
	return window;
}

void KCF::init(InputArray frame, Rect rect) {
	center = RectTools::center(rect);
	target_size = rect.size();
	window_size = target_size * params.search_window_scale;
	feed_size = best_suited_feed_size(window_size);
	
	window = update_sample_window();
	
	// init feature
	feature = HogFeature(feed_size);
	
	size = feature.output_size();
	sizef.height = size.height;
	sizef.width = size.width/2+1;
	
	Mat sampled_window = RectTools::subwindow(frame.getMat(), window, cv::BORDER_REPLICATE);
	resize(sampled_window, sampled_window, feed_size);
	sampled_window.convertTo(feature.input(), CV_32FC3);
	
	feature.compute();
	
	pca = PCA(feature.output(), Mat(), PCA::DATA_AS_COL, 10);
	
	pca.project(feature.output(), pcadst);
	CHECK_EQ(pcadst.rows, 10);
	
	// xf = fft(x)
	int n[2] = {size.height, size.width};
	xf_eg.init(pcadst, n);
	xf_eg.compute();
	xf = xf_eg.dst;

	// yf = fft(y)
	Mat ys = gaussian_label(size, 30);
	ifftshift(ys, ys);
	Mat y = Mat(1, size.height * size.width, CV_32F, ys.data);
	yf_eg.init(y, n);
	yf_eg.compute();
	yf = yf_eg.dst;
	
	hf.create(xf.rows, xf.cols, xf.type());
	
	train(); // use xf and yf for training -> hf
	test();
	
}

void KCF::update(InputArray frame) {
	window = update_sample_window();
	LOG(INFO) << window;
	
	Mat sampled_window = RectTools::subwindow(frame.getMat(), window, cv::BORDER_REPLICATE);
	resize(sampled_window, sampled_window, feed_size);
	sampled_window.convertTo(feature.input(), CV_32FC3);
	
	feature.compute();

	pca.project(feature.output(), pcadst);
	
	xf_eg.compute();
	
	mulSpectrums(hf, xf, rf, 0, false);
//	int n[2] = {size.height, size.width};

	rf_eg.compute();
	
	// sum over channels
	Mat resp;
	reduce(rf_eg.dst, resp, 0, REDUCE_SUM);
	Mat r = Mat(size, CV_32F, resp.data);
	
	fftshift(r, r);
	
	imagesc("r", r);
	
	Point2i pi;
	double pv;
	minMaxLoc(r, NULL, &pv, NULL, &pi);
	double peak_value = (float) pv;
	
	//subpixel peak estimation, coordinates will be non-integer
	cv::Point2f p((float)pi.x, (float)pi.y);
	
	if (pi.x > 0 && pi.x < r.cols-1) {
		p.x += subPixelPeak(r.at<float>(pi.y, pi.x-1), peak_value, r.at<float>(pi.y, pi.x+1));
	}
	
	if (pi.y > 0 && pi.y < r.rows-1) {
		p.y += subPixelPeak(r.at<float>(pi.y-1, pi.x), peak_value, r.at<float>(pi.y+1, pi.x));
	}
	
	p.x -= (r.cols) / 2;
	p.y -= (r.rows) / 2;
	
	center[0] += p.y * 4;
	center[1] += p.x * 4;
}

void KCF::train() {
	Mat nume, deno;
	for (int i = 0; i < 10; i++) {
		Mat xf_ = xf.row(i);
		Mat hf_ = hf.row(i);
		mulSpectrums(yf, xf_, nume, 0, true);
		mulSpectrums(xf_, xf_, deno, 0, true);
		FFTTools::complexDivision(nume, deno + 0.01, hf_);
	}
}

void KCF::test() {
	mulSpectrums(hf, xf, rf, 0, false);
	int n[2] = {size.height, size.width};
//	copyMakeBorder(rf, rf_padded, n[0]*2, n[0]*2, n[1]*2, 0, BORDER_CONSTANT, Scalar(0));
	rf_eg.init(rf, n);
}

Rect KCF::getrect() {
	Rect r;
	r.width = target_size.width;
	r.height = target_size.height;
	r.x = center[0] - r.width / 2;
	r.y = center[1] - r.height / 2;
	return r;
}
