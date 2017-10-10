//
//  fourier_coefficients.hpp
//  vt
//
//  Created by Yizhuang Zhou on 07/10/2017.
//  Copyright © 2017 Yizhuang Zhou. All rights reserved.
//

#ifndef fourier_coefficients_hpp
#define fourier_coefficients_hpp

#include <stdio.h>
#include <opencv2/core.hpp>

#include <glog/logging.h>

using namespace cv;
using std::complex;

// src should be center highlighted
void full_fourier_coefficients(InputArray src, OutputArray dst) {
	CHECK_EQ(src.type(), CV_32FC2) << "InputArray TYPE ERROR";
	Mat lh = src.getMat();
	Mat llh(lh, Range::all(), Range(0,lh.cols-1));
	Mat rh;
	rotate(llh, rh, ROTATE_180);
	rh.forEach<complex<float>>([](complex<float> &c, const void*) -> void { c.imag(-c.imag()); });
	hconcat(lh, rh, dst);
}

// src should be center highlighted
void compact_fourier_coefficients(InputArray src, OutputArray dst) {
	CHECK_EQ(src.type(), CV_32FC2) << "InputArray TYPE ERROR";
	dst.assign(Mat(src.getMat(), Range::all(), Range(0,(src.cols()+1)/2)));
}

#endif /* fourier_coefficients_hpp */
