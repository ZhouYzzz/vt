//
//  unittest-utility.cpp
//  vt
//
//  Created by Yizhuang Zhou on 23/09/2017.
//  Copyright © 2017 Yizhuang Zhou. All rights reserved.
//

#include <stdio.h>

#include "gtest/gtest.h"
#include <glog/logging.h>

#include "circshift.hpp"
#include "fft.hpp"

TEST(Util, DISABLED_Circshift) {
	cv::Mat A = cv::Mat(9,11,CV_32F,cv::Scalar(0));
	A.at<float>(0, 0) = 1;
	LOG(INFO) << A;
//	circshift(A, A, -1, -1);
	fftshift(A);
	LOG(INFO) << A;
//	LOG(INFO) << -7 % 3;
}

TEST(Util, DISABLED_FFT) {
	cv::Mat A = cv::Mat(3,3,CV_32F,cv::Scalar(0));
	A.at<float>(0,0) = 1;
	cv::Mat Af;
	fft2(A, Af);
	cv::Mat plain[2];
	cv::split(Af, plain);
	
	LOG(INFO) << '\n' << plain[0];
	LOG(INFO) << '\n' << plain[1];
}
