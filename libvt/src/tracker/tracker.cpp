//
//  tracker.cpp
//  vt
//
//  Created by Yizhuang Zhou on 23/09/2017.
//  Copyright © 2017 Yizhuang Zhou. All rights reserved.
//

#include "tracker.hpp"
#include "recttools.hpp"
#include "ffttools.hpp"
#include "fft.hpp"
#include "hog.hpp"
#include "label.hpp"
#include "circshift.hpp"
#include "imagesc.hpp"

DCF::DCF(parameter params) : Tracker(params) {}

void DCF::init(cv::Mat frame, cv::Rect rect) {
	using namespace cv;
	// construct target and window size
	center = RectTools::center(rect);
	target_size = rect.size();
	window_size = target_size * params.search_window_scale;
	
	// update window
	window.width = window_size.width;
	window.height = window_size.height;
	window.x = center[0] - window.width / 2;
	window.y = center[1] - window.height / 2;
	
	// find best suited feed_sz (can be divided by cellsize);
	int feed_sz_ = (floorf(sqrt(window_size.area())/4/2)*2+1)*4;
	feed_size = Size2i(feed_sz_, feed_sz_);
	
	// construct feature engine
	feature = Hog(feed_size);
	
	Mat sampled_window = RectTools::subwindow(frame, window, cv::BORDER_REPLICATE);
	resize(sampled_window, resized_window, feed_size);
	resized_window.convertTo(feature.input(), CV_32FC3);
	
	// compute feature
	feature.compute();
	
	y = gaussian_label(feature.output_size()[0], 0.5*feature.output_size()[0].width);
	ifftshift(y);
	fft2(y, yf);
	
	auto output_size = feature.output_size()[0];
	auto output_channel = feature.output_channel()[0];
	auto output_step = feature.output()[0].step[0]; // channel stride, in bytes
	
	xf = Mat(output_channel, output_size.height * output_size.width, CV_32FC2);
	hf = Mat(output_channel, output_size.height * output_size.width, CV_32FC2);
	
	Mat A, B;
	// for each channel
	for (int i = 0; i < output_channel; i++) {
		Mat x_ = Mat(output_size, CV_32F, feature.output()[0].ptr()+i*output_step);
		Mat xf_ = Mat(output_size, CV_32FC2, xf.ptr()+i*output_step*2);
		Mat hf_ = Mat(output_size, CV_32FC2, hf.ptr()+i*output_step*2);
		fft2(x_, xf_);

		mulSpectrums(yf, xf_, A, 0, true);
		mulSpectrums(xf_, xf_, B, 0, true);
		FFTTools::complexDivision(A, B + params.lambda, hf_);
		
//		mulSpectrums(hf_, xf_, A, 0, false);
//		ifft2(A);
//		Mat resp = FFTTools::real(A);
//		ifftshift(resp);
//		imagesc("outcome", resp);
	}
}

void DCF::update(cv::Mat frame) {
	Mat sampled_window = RectTools::subwindow(frame, window, cv::BORDER_REPLICATE);
	resize(sampled_window, resized_window, feed_size);
	resized_window.convertTo(feature.input(), CV_32FC3);
	
	// compute feature
	feature.compute();

	// detect
	auto output_size = feature.output_size()[0];
	auto output_channel = feature.output_channel()[0];
	auto output_step = feature.output()[0].step[0]; // channel stride, in bytes

	Mat tmp;
	Mat resp = Mat(output_size, CV_32F, Scalar(0));
	// for each channel
	for (int i = 0; i < output_channel; i++) {
		Mat x_ = Mat(output_size, CV_32F, feature.output()[0].ptr()+i*output_step);
		Mat xf_ = Mat(output_size, CV_32FC2, xf.ptr()+i*output_step*2);
		Mat hf_ = Mat(output_size, CV_32FC2, hf.ptr()+i*output_step*2);
		fft2(x_, xf_);
		mulSpectrums(hf_, xf_, tmp, 0, false);
		ifft2(tmp);
		fftshift(tmp);
		
		resp = resp + FFTTools::real(tmp);
	}
//	imagesc("response", resp);
	
	double maxv;
	Point2i maxp;
	minMaxLoc(resp, NULL, &maxv, NULL, &maxp);
	
//	LOG(INFO) << "MAXV:" << maxv << " LOC" << maxp;
	
	center[0] += 4*(maxp.x - (output_size.width/2));
	center[1] += 4*(maxp.y - (output_size.height/2));
	
	// update window
	window.width = window_size.width;
	window.height = window_size.height;
	window.x = center[0] - window.width / 2;
	window.y = center[1] - window.height / 2;
	
	// update model
	// WIP
}

Rect DCF::getrect() {
	Rect2f rect;
	rect.width = target_size.width;
	rect.height = target_size.height;
	rect.x = center[0] - rect.width/2;
	rect.y = center[1] - rect.height/2;
	return rect;
}
