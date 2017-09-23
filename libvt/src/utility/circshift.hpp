//
//  circshift.hpp
//  vt
//
//  Created by Yizhuang Zhou on 21/09/2017.
//  Copyright © 2017 Yizhuang Zhou. All rights reserved.
//

#ifndef circshift_hpp
#define circshift_hpp

#include <stdio.h>

#include <opencv2/core.hpp>

void circshift(cv::InputArray src, cv::OutputArray dst, int xshift, int yshift);

void fftshift(cv::InputArray src, cv::OutputArray dst);
void ifftshift(cv::InputArray src, cv::OutputArray dst);
void fftshift(cv::InputOutputArray src);
void ifftshift(cv::InputOutputArray src);

#endif /* circshift_hpp */
