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

cv::Mat gaussian_label(cv::Size2i sz, float sigma);

#endif /* label_hpp */
