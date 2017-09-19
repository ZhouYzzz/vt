//
//  label.hpp
//  vt
//
//  Created by Yizhuang Zhou on 19/09/2017.
//  Copyright © 2017 Yizhuang Zhou. All rights reserved.
//

#ifndef label_hpp
#define label_hpp

#include <stdio.h>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

cv::Mat gaussian_label(cv::Size2i sz, float sigma) {
	cv::Mat res(sz, CV_32F);
	
	int syh = (sz.height) / 2;
	int sxh = (sz.width) / 2;
	
	float output_sigma = std::sqrt((float) sz.height * sz.width) / sigma;
	float mult = -0.5 / (output_sigma * output_sigma);
	
	for (int i = 0; i < sz.height; i++)
		for (int j = 0; j < sz.width; j++) {
			int ih = i - syh;
			int jh = j - sxh;
			res.at<float>(i, j) = std::exp(mult * (float) (ih * ih + jh * jh));
		}
	return res;
}

#endif /* label_hpp */
