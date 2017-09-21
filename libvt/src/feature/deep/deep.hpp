//
//  deep.hpp
//  vt
//
//  Created by Yizhuang Zhou on 19/09/2017.
//  Copyright © 2017 Yizhuang Zhou. All rights reserved.
//

#ifndef deep_hpp
#define deep_hpp

#include <stdio.h>

#include "feature.hpp"

class Deep : public Feature {
	
public:
	Deep() {}
	~Deep() {}
	
	void compute();
	void reshape(cv::Size sz);
};

#endif /* deep_hpp */
