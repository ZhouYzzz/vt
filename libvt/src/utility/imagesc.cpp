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
//	double min, max;
//	cv::minMaxLoc(m, &min, &max);
//	cv::Mat norm = (m - min) / (max - min + DBL_EPSILON);
	cv::Mat norm, display;
	cv::normalize(m, norm);
	norm.convertTo(display, CV_8UC1, 255, 0);
	cv::applyColorMap(display, display, cv::COLORMAP_JET);
	cv::imshow(winname, display);
	cv::waitKey();
}
