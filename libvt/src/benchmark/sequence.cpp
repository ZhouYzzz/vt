//
//  sequence.cpp
//  vt
//
//  Created by Yizhuang Zhou on 17/09/2017.
//  Copyright © 2017 Yizhuang Zhou. All rights reserved.
//

#include "sequence.hpp"

bool sequence::unfinished() {
	return current_frame < end_frame;
}

cv::Rect sequence::get_init_rect() {
	return ground_truth_rects[0];
}

cv::Mat sequence::get_next_frame() {
	CHECK_EQ(unfinished(), true);
	return cv::imread(image_files[current_frame++]);
}
