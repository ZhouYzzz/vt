//
//  hog.hpp
//  vt
//
//  Created by Yizhuang Zhou on 19/09/2017.
//  Copyright © 2017 Yizhuang Zhou. All rights reserved.
//

#ifndef hog_hpp
#define hog_hpp

#include <stdio.h>

#include "feature.hpp"

using namespace cv;

class Hog : public Feature {
	
public:
	Hog() {}
	~Hog() {}
	Hog(Size2i input_sz);

	int downsample_factor() { return 4; }
	void compute();
};

struct hog_parameter {
//	bool dim_reduction = true;
//	int compressed_dims = 10;
};

class HogFeature {
private:
	Mat src; // input
	Mat win; // cosine window
	Mat dst; // output
	
	Size2i output_sz;
	
	hog_parameter params;
public:
	HogFeature();
	~HogFeature();
	
	HogFeature(Size2i, hog_parameter = hog_parameter());
	
	Mat & input() { return src; }
	Mat & output() { return dst; }
	
	Size2i output_size() { return output_sz; }
	
	void compute();
};

#endif /* hog_hpp */
