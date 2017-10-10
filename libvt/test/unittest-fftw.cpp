//
//  unittest-fftw.cpp
//  vt
//
//  Created by Yizhuang Zhou on 18/09/2017.
//  Copyright © 2017 Yizhuang Zhou. All rights reserved.
//

#include <stdio.h>

#include "gtest/gtest.h"

#include <glog/logging.h>
#include <fftw3.h>
#include <opencv2/core.hpp>

//#include "ffttools.hpp"
#include "label.hpp"
#include "circshift.hpp"

TEST(FFTW3, DISABLED_Gaussian) {
//	cv::Mat y =  gaussian_label(cv::Size2i(7,5),10);
//	FFTTools::rearrange( y );
//	LOG(INFO) << y;
}

TEST(FFTW3, DISABLED_TEST) {
//	float in[] = {0,1,2,3,4};
//	fftwf_complex *out = new fftwf_complex[3];
//	fftwf_plan plan = fftwf_plan_dft_r2c_1d(5, in, out, FFTW_ESTIMATE);
//	fftwf_execute(plan);
//	
//	for (int i = 0; i < 3; i++)
//		std::cout << out[i][0] << " + i * " << out[i][1] << std::endl;
//	
//	fftwf_destroy_plan(plan);
	LOG(INFO) << sizeof(fftwf_complex) << ' ' << sizeof(float);
	
	float in[] = {0,1,2,3,4,5,6,7,9};
	fftwf_complex *out = new fftwf_complex[6];
	
	fftwf_plan plan = fftwf_plan_dft_r2c_2d(3, 3, in, out, FFTW_ESTIMATE);
	fftwf_execute(plan);
	
	for (int i = 0; i < 6; i++)
		std::cout << out[i][0] << " + i * " << out[i][1] << std::endl;
	
	fftwf_destroy_plan(plan);
}

TEST(FFTW3, DISABLED_CV_DFT) {
	float in[] = {0,1,2,3,4,5,6,7,8};
	cv::Mat A[] = {cv::Mat(3,3,CV_32F,in), cv::Mat::zeros(3, 3, CV_32F)};
	cv::Mat Ac;
	cv::merge(A, 2, Ac);
	cv::Mat Af;
	cv::dft(Ac, Af);
	
	cv::split(Af, A);
	
	LOG(INFO) << A[0];
	
//	FFTTools::rearrange(A[0]);
	
	LOG(INFO) << A[0];
}

TEST(FFTW3, DISABLED_CIRCSHIFT) {
	auto a = cv::Mat(5,5,CV_32F,cv::Scalar(0));
	a.at<float>(1, 1) = 1;
	cv::Mat1f b;
	a.copyTo(b);
	
//	auto c = cv::Mat(5,5,CV_32F,cv::Scalar(0));
//	cv::Mat d;
//	c.copyTo(d);
	
//	cv::Mat b = cv::Mat(5,5,CV_32F,a.ptr<float>());
	
//	b.create(5, 5, CV_32F);
//	circshift<float>(a, a, 1, 1);
	LOG(INFO) << a;
}
