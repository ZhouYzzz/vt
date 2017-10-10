//
//  main.cpp
//  main
//
//  Created by Yizhuang Zhou on 18/09/2017.
//  Copyright © 2017 Yizhuang Zhou. All rights reserved.
//

#include <iostream>

#include <glog/logging.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "benchmark.hpp"
#include "parameters.hpp"
#include "tracker.hpp"
#include "trackerKCF.hpp"

int main(int argc, const char * argv[]) {
	parameter params;
	
	std::string sequence_name = "DragonBaby";
	sequence * s = OTB::construct("/Users/zhouyz/Development/Data/OTB50", sequence_name);
	
	// init first frame
	cv::Mat im;
	cv::Rect2f rect;
	im = s->get_current_frame();
	rect = s->get_init_rect();
	
//	DCF tracker(params);
	KCF tracker(params);
	
	tracker.init(im, rect);
	
	s->step();
	
	if (params.visualization) {
		cv::rectangle(im, rect, cv::Scalar(0,0,255), 2); // draw groundtruth rect
		cv::imshow(sequence_name, im);
		cv::waitKey();
	}
	
	// detect remaining frames
	cv::TickMeter tm;
	tm.start();
	
	while (s->unfinished()) {
		im = s->get_current_frame();
		
		tracker.update(im);
		
		if (params.visualization) {
			cv::rectangle(im, s->ground_truth_rects[s->current_frame], cv::Scalar(0,255,0), 2); // draw groundtruth rect
			cv::rectangle(im, tracker.getrect(), cv::Scalar(0,0,255), 2); // draw detected rect
			cv::imshow(sequence_name, im);
			cv::waitKey();
		}
		// next frame
		s->step();
	}
	
	tm.stop();
	LOG(INFO) << "FPS:" << s->current_frame / tm.getTimeSec();
	return 0;
}
