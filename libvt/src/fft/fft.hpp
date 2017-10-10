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

using namespace cv;

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

struct cfft2 {
	Mat src; // of size [C][H*W]
	Mat dst; // of size [C][H*W/2]
	fftwf_plan plan;
	int rows, cols;
	
	cfft2();
	~cfft2();
	
	void init(InputArray, const int*); // size [H,W]
	void compute();
	void rearrange();
	void rearrange2();
};

struct cifft2 {
	Mat src; // of size [C][H*W/2]
	Mat dst; // of size [C][H*W]
	fftwf_plan plan;
	int rows, cols;
	
	cifft2();
	~cifft2();
	
	void init(InputArray, const int*); // size [H,W]
	void compute();
	void rearrange();
};

void fft2(cv::InputArray src, cv::OutputArray dst);
void ifft2(cv::InputArray src, cv::OutputArray dst);
void fft2(cv::InputOutputArray src);
void ifft2(cv::InputOutputArray src);
//void cfft2(cv::InputArray src, cv::OutputArray dst);
//void cifft2(cv::InputArray src, cv::OutputArray dst);

#endif /* fft_hpp */
