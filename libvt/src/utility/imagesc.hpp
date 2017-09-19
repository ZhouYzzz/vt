//
//  imagesc.hpp
//  vt
//
//  Created by Yizhuang Zhou on 19/09/2017.
//  Copyright © 2017 Yizhuang Zhou. All rights reserved.
//

#ifndef imagesc_hpp
#define imagesc_hpp

#include <stdio.h>

#include <glog/logging.h>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

void imagesc(const cv::String &winname, cv::Mat m);

#endif /* imagesc_hpp */
