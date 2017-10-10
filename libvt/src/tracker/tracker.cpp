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

using namespace cv;

DCF::DCF(parameter params) : Tracker(params) {}

void DCF::init_feature_engine(Size2i input_size, bool gray_scale) {
	feature = Hog(input_size);
}

Size2i DCF::best_suited_feed_size(Size2f input_size, std::string mode) {
	if (strcmp(mode.data(), "square")) {
		int cellsize = feature.downsample_factor();
		int feed_sz_ = (floorf(sqrt(input_size.area())/cellsize/2)*2+1)*cellsize; // round to even number
		return Size2i(feed_sz_, feed_sz_);
	} else if (strcmp(mode.data(), "test")) {
		int cellsize = feature.downsample_factor();
		int feed_h_ = (ceilf(input_size.height/cellsize/2)*2+1)*cellsize * 3;
		int feed_w_ = (ceilf(input_size.width/cellsize/2)*2+1)*cellsize * 3;
		return Size2i(feed_h_, feed_w_);
	} else {
		LOG(FATAL) << "No search window mode " << mode;
	}
}

Rect2f DCF::update_sample_window() {
	window.width = window_size.width;
	window.height = window_size.height;
	window.x = center[0] - window.width / 2;
	window.y = center[1] - window.height / 2;
	return window;
}

void DCF::preallocatemem(Size2i output_size, int output_channel) {
	xf.create(output_channel, output_size.height * output_size.width, CV_32FC2);
	hf.create(output_channel, output_size.height * output_size.width, CV_32FC2);
}

void DCF::init(cv::Mat frame, cv::Rect rect) {
	// construct target and window size
	center = RectTools::center(rect);
	target_size = rect.size();
	window_size = target_size * params.search_window_scale;
	
	// update window
	window = update_sample_window();
	
//	// find best suited feed_sz (can be divided by cellsize);
//	int feed_sz_ = (floorf(sqrt(window_size.area())/4/2)*2+1)*4;
//	feed_size = Size2i(feed_sz_, feed_sz_);
	
	// construct feature engine
//	feature = Hog(best_suited_feed_size(window_size));
	
	feed_size = best_suited_feed_size(window_size);
	init_feature_engine(feed_size);

	// construct regression label
	Mat y = gaussian_label(feature.output_size()[0], 0.5*feature.output_size()[0].width);
	ifftshift(y);
	fft2(y, yf);
	
	Mat sampled_window = RectTools::subwindow(frame, window, cv::BORDER_REPLICATE);
	resize(sampled_window, resized_window, feed_size);
	resized_window.convertTo(feature.input(), CV_32FC3);
	
	// compute feature
	feature.compute();
	
	auto output_size = feature.output_size()[0];
	auto output_channel = feature.output_channel()[0];
	auto output_step = feature.output()[0].step[0]; // channel stride, in bytes
	
	PCACompute(feature.output()[0], projection_mean, projection_matrix, 10);
	
	PCAProject(feature.output()[0], projection_mean, projection_matrix, projected_feature);
	
	output_channel = 10;
	
	preallocatemem(output_size, output_channel);

	Mat A, B; // tmp array
	// perform fft and training
	for (int i = 0; i < output_channel; i++) {
		Mat x_ = Mat(output_size, CV_32F, projected_feature.ptr()+i*output_step);
		Mat xf_ = Mat(output_size, CV_32FC2, xf.ptr()+i*output_step*2);
		Mat hf_ = Mat(output_size, CV_32FC2, hf.ptr()+i*output_step*2);
		fft2(x_, xf_);

		mulSpectrums(yf, xf_, A, 0, true);
		mulSpectrums(xf_, xf_, B, 0, true);
		FFTTools::complexDivision(A, B + params.lambda, hf_);
	}
}

void DCF::update(cv::Mat frame) {
	window = update_sample_window();

	Mat sampled_window = RectTools::subwindow(frame, window, cv::BORDER_REPLICATE);
	resize(sampled_window, resized_window, feed_size);
	resized_window.convertTo(feature.input(), CV_32FC3);
	
	// compute feature
	feature.compute();

	// detect
	auto output_size = feature.output_size()[0];
	auto output_channel = feature.output_channel()[0];
	auto output_step = feature.output()[0].step[0]; // channel stride, in bytes

	PCAProject(feature.output()[0], projection_mean, projection_matrix, projected_feature);
	
	output_channel = 10;
	
	Mat tmp;
	Mat resp = Mat(output_size, CV_32F, Scalar(0));
	// for each channel
	for (int i = 0; i < output_channel; i++) {
		Mat x_ = Mat(output_size, CV_32F, projected_feature.ptr()+i*output_step);
		Mat xf_ = Mat(output_size, CV_32FC2, xf.ptr()+i*output_step*2);
		Mat hf_ = Mat(output_size, CV_32FC2, hf.ptr()+i*output_step*2);
		fft2(x_, xf_);
		mulSpectrums(hf_, xf_, tmp, 0, false);
		ifft2(tmp);
		fftshift(tmp);
		
		resp = resp + FFTTools::real(tmp);
	}
	
	if (params.debug) {
		imagesc("response", resp);
	}
	
	// maximum resp -> location change
	double maxv;
	Point2i maxp;
	minMaxLoc(resp, NULL, &maxv, NULL, &maxp);
		
	center[0] += 4*(maxp.x - (output_size.width/2));
	center[1] += 4*(maxp.y - (output_size.height/2));
	
	// update window
	window = update_sample_window();
	
	// extract feature and update model
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
