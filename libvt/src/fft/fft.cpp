//
//  fft.cpp
//  vt
//
//  Created by Yizhuang Zhou on 19/09/2017.
//  Copyright © 2017 Yizhuang Zhou. All rights reserved.
//

#include <glog/logging.h>

#include "fft.hpp"

fft2::fft2() {}

fft2::~fft2() {
	fftwf_destroy_plan(plan);
	delete [] out;
}

void fft2::construct(float *src, cv::Size2i sz, int many) {
	if (sz.width != sz.height) abort();
	int n[2] = { sz.height, sz.width };
	in = src;
	out = fftwf_alloc_complex(n[0]*(n[1]/2+1)*many);
	plan = fftwf_plan_many_dft_r2c(
								   2, n, many,
								   in, NULL, 1, n[0]*n[1],
								   out, NULL, 1, (n[0]*n[1]/2+1),
								   FFTW_ESTIMATE);
}

void fft2::get_output(std::vector<cv::Mat> &out_mat, cv::MatSize n) {
	out_mat.clear();
	fftwf_complex * tmp_ptr = out;
	for (int c = 0; c < 31; c++) {
		cv::Mat m = cv::Mat(n[0],n[1]/2+1, CV_32FC2, tmp_ptr);
		out_mat.push_back(m);
		tmp_ptr += n[0]*(n[1]/2+1);
	}
}

void fft2::compute() {
	fftwf_execute(plan);
}

void fft2(cv::InputArray src, cv::OutputArray dst) {
	using namespace cv;
	if (src.getObj() == dst.getObj()) {
		CHECK_EQ(src.channels(), 2) << "Inplace fft requires src a complex mat";
		dft(src, dst);
	} else {
		if (src.channels() == 1) {
			Mat plain[2] = {src.getMat(), Mat::zeros(src.size(), src.type())};
			merge(plain, 2, dst);
		} else {
			CHECK_EQ(src.channels(), 2) << "Outplace fft requires src's channel <= 2";
		}
		dft(dst, dst);
	}
}

void ifft2(cv::InputArray src, cv::OutputArray dst) {
	using namespace cv;
	if (src.getObj() == dst.getObj()) {
		CHECK_EQ(src.channels(), 2) << "Inplace fft requires src a complex mat";
		idft(src, dst);
	} else {
		if (src.channels() == 1) {
			Mat plain[2] = {src.getMat(), Mat::zeros(src.size(), src.type())};
			merge(plain, 2, dst);
		} else {
			CHECK_EQ(src.channels(), 2) << "Outplace fft requires src's channel <= 2";
		}
		idft(dst, dst);
	}
}

void fft2(cv::InputOutputArray src) { fft2(src, src); }
void ifft2(cv::InputOutputArray src) { ifft2(src, src); }
