//
//  hann.cpp
//  vt
//
//  Created by Yizhuang Zhou on 19/09/2017.
//  Copyright © 2017 Yizhuang Zhou. All rights reserved.
//

#include "hann.hpp"

cv::Mat hann(cv::Size2i sz) {
	cv::Mat hann;
	cv::Mat hann1t = cv::Mat(cv::Size(sz.height,1), CV_32F);
	cv::Mat hann2t = cv::Mat(cv::Size(1,sz.width), CV_32F);
	for (int i = 0; i < hann1t.cols; i++)
		hann1t.at<float>(i) = 0.5 * (1 - cos(2 * CV_PI * (i + 1) / (hann1t.cols + 1)));
	for (int i = 0; i < hann2t.rows; i++)
		hann2t.at<float>(i) = 0.5 * (1 - cos(2 * CV_PI * (i + 1) / (hann2t.rows + 1)));
	hann = hann2t * hann1t;
	return hann;
}
