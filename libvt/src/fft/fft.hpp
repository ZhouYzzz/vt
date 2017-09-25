//
//  fft.hpp
//  vt
//
//  Created by Yizhuang Zhou on 19/09/2017.
//  Copyright © 2017 Yizhuang Zhou. All rights reserved.
//

#ifndef fft_hpp
#define fft_hpp

#include <stdio.h>

#include <fftw3.h>
#include <opencv2/core.hpp>

struct fft2 {
	float * in;
	fftwf_complex * out;
	fftwf_plan plan;
	
	fft2();
	~fft2();
	
	void construct(float * src, cv::Size2i sz, int many);
	void get_output(std::vector<cv::Mat> & out_mat, cv::MatSize);
	void compute();
};

struct ifft2 {
	fftwf_complex * in;
	float * out;
	fftwf_plan plan;
};

void fft2(cv::InputArray src, cv::OutputArray dst);
void ifft2(cv::InputArray src, cv::OutputArray dst);
void fft2(cv::InputOutputArray src);
void ifft2(cv::InputOutputArray src);

#endif /* fft_hpp */
