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

class Hog : public Feature {
	
public:
	Hog() {}
	~Hog() {}
	Hog(cv::Size2i input_sz);

	void compute();
};

#endif /* hog_hpp */
