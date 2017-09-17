//
//  sequence.hpp
//  vt
//
//  Created by Yizhuang Zhou on 17/09/2017.
//  Copyright © 2017 Yizhuang Zhou. All rights reserved.
//

#ifndef sequence_hpp
#define sequence_hpp

#include <stdio.h>
#include <iostream>
#include <vector>

#include <glog/logging.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

struct sequence {
	std::string name;
	int start_frame = 0;
	int end_frame = 0;
	int current_frame = 0;
	
	std::vector<std::string> image_files;
	std::vector<cv::Rect> ground_truth_rects;
	std::vector<cv::Rect> result_rects;
	
	bool unfinished();
	cv::Rect get_init_rect();
	cv::Mat get_next_frame();
};

#endif /* sequence_hpp */
