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
	std::vector<cv::Rect> rects;
	parameter params;
	
public:
	Tracker() {}
	~Tracker() {}
	Tracker(parameter params) : params(params) {}
	virtual void init(cv::Mat frame, cv::Rect rect) {}
	virtual void update(cv::Mat frame) {}
};

class DCF : public Tracker {
	Hog feature;
	Mat hf;
	Mat xf;
	Mat y, yf;
	Mat resp;
	
	Vec2f center;
	Rect2f window;
	Mat resized_window;
	Size2i feed_size; // size to feed feature engine
	Size2f target_size;
	Size2f window_size;
public:
	DCF() {}
	~DCF() {}
	DCF(parameter params);
	void init(cv::Mat frame, cv::Rect rect);
	void update(cv::Mat frame);
	Rect getrect();
};

#endif /* tracker_hpp */
