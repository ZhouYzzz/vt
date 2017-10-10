//
//  trackerKCF.hpp
//  vt
//
//  Created by Yizhuang Zhou on 25/09/2017.
//  Copyright © 2017 Yizhuang Zhou. All rights reserved.
//

#ifndef trackerKCF_hpp
#define trackerKCF_hpp

#include <stdio.h>

#include <glog/logging.h>

#include "common.hpp"
#include "fft.hpp"
#include "tracker.hpp"
#include "parameters.hpp"
#include "hog.hpp"

using namespace cv;

class KCF : public Tracker {
	Mat hf; // learned filters, [C][H][W/2]c
	Mat xf; // feature samples(f), [C][H][W/2]c
	Mat yf; // label(f), [1][H][W/2]c
	Mat rf; // response(f)
//	Mat rf_padded;
	
	cfft2 xf_eg;
	cfft2 yf_eg;
	cifft2 rf_eg;
	
	Vec2f center;	// target center location, updated over time
	Rect2f window;	// search window
//	Size2f init_target_size;
//	Size2f init_window_size;
//	float current_scale_factor;
	Size2f target_size; // current target size
	Size2f window_size; // current window size
	Size2i feed_size; // size to feed feature engine
	
	Size2i size; // size of data patch, [H][W]
	Size2i sizef; // size of fourier, [H][W/2+1]
	int channel; // channel of data patch
	
	HogFeature feature;
	PCA pca;
	Mat pcadst;
	
	parameter params;
private:
	void train();
	void test();
	Size2i best_suited_feed_size(Size2f, string = "test");
	Rect2f update_sample_window();
public:
	KCF() {}
	~KCF() {}
	KCF(parameter);
	
	void init(InputArray frame, Rect rect);
	void update(InputArray frame);
	Rect getrect();
};

#endif /* trackerKCF_hpp */
