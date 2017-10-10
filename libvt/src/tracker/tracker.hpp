//
//  tracker.hpp
//  vt
//
//  Created by Yizhuang Zhou on 23/09/2017.
//  Copyright © 2017 Yizhuang Zhou. All rights reserved.
//

#ifndef tracker_hpp
#define tracker_hpp

#include <stdio.h>
#include <vector>

#include <opencv2/core.hpp>

#include "hog.hpp"
#include "parameters.hpp"

using namespace cv;

class Tracker {
protected:
	parameter params;
	
public:
	Tracker() {}
	~Tracker() {}
	Tracker(parameter params) : params(params) {}
	virtual void init(cv::Mat frame, cv::Rect rect) {}
	virtual void update(cv::Mat frame) {}
};

class DCF : public Tracker {
	/* feature engine
	 cropped_image -> [M*input algorithm M*output] -> fftengine
	 input  : [iH, iW (C=3)]
	 output : [oC, oH*oW (1)]
	 */
	Hog feature;
	
	/* inner mats, contains mem blocks */
	Mat hf;		// [C, H*W]
	Mat xf;		// [C, H*W]
	Mat yf;		// [H, W]
	
	Mat projection_matrix;
	Mat projection_mean;
	Mat projected_feature;
	
	Mat resized_window; // input image window, can be visualized
	
	Vec2f center;	// target center location, updated over time
	Rect2f window;	// search window
	Size2f init_target_size;
	Size2f init_window_size;
	float current_scale_factor;
	Size2f target_size; // current target size
	Size2f window_size; // current window size
	
	Size2i feed_size; // size to feed feature engine

private:
	void init_feature_engine(Size2i input_size, bool gray_scale = false);
	Size2i best_suited_feed_size(Size2f input_size, std::string mode = "test");
	Rect2f update_sample_window();
	void preallocatemem(Size2i output_size, int output_channel);
	void prepare_feature(Mat cropped_image);
public:
	DCF() {}
	~DCF() {}
	DCF(parameter params);
	void init(Mat frame, Rect rect);
	void update(Mat frame);
	Rect getrect();
};

#endif /* tracker_hpp */
