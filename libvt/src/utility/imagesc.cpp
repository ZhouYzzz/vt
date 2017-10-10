//
//  imagesc.cpp
//  vt
//
//  Created by Yizhuang Zhou on 19/09/2017.
//  Copyright © 2017 Yizhuang Zhou. All rights reserved.
//

#include "imagesc.hpp"

void imagesc(const cv::String &winname, cv::Mat m) {
	if (m.channels() != 1)
		LOG(FATAL) << "Should pass single channel mat";
	
	double min;
	double max;
	cv::minMaxIdx(m, &min, &max);
	cv::Mat display;
	// Histogram Equalization
	float scale = 255 / (max-min);
	m.convertTo(display,CV_8UC1, scale, -min*scale);
	
	cv::applyColorMap(display, display, cv::COLORMAP_JET);
	cv::resize(display, display, cv::Size2i(400,400), 0,0,cv::INTER_NEAREST);
	cv::imshow(winname, display);
	cv::waitKey();
}
