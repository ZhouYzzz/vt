//
//  feature.hpp
//  vt
//
//  Created by Yizhuang Zhou on 17/09/2017.
//  Copyright © 2017 Yizhuang Zhou. All rights reserved.
//

#ifndef feature_hpp
#define feature_hpp

#include <stdio.h>
#include <vector>

#include <opencv2/core.hpp>

class Feature {
protected:
	uchar * in_; // input data ptr
	cv::Mat input_; // input raw image (H,W,C=3)

	int n_out_; // num of feature cells (vector.size())
	std::vector<float*> out_; // output data ptr (different size)
	std::vector<cv::Mat> output_; // output features (C,H*W,1)
	
	std::vector<int> out_channel_; // output data num of channel
	std::vector<cv::Size2i> out_size_; // output spatial size (H,W), W-major
	
	std::vector<cv::Mat> hann_;
	
public:
	Feature() {}
	~Feature() {}
	
	cv::Mat & input() { return input_; }
	std::vector<cv::Mat> & output() { return output_; }
	std::vector<cv::Size2i> & output_size() { return out_size_; }
	std::vector<int> & output_channel() { return out_channel_; }
	int num_output() { return n_out_; }
//	virtual void compute() {}
//	virtual void reshape(cv::Size sz) {}
};

#endif /* feature_hpp */
