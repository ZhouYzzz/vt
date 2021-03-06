//
//  parameters.hpp
//  vt
//
//  Created by Yizhuang Zhou on 18/09/2017.
//  Copyright © 2017 Yizhuang Zhou. All rights reserved.
//

#ifndef parameters_hpp
#define parameters_hpp

#include <stdio.h>

struct parameter {
	bool visualization = true;
	bool debug = true;
	
	float search_window_scale = 4;
	
	float lambda = 1e-3;
};

#endif /* parameters_hpp */
