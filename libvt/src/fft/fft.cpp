//
//  fft.cpp
//  vt
//
//  Created by Yizhuang Zhou on 19/09/2017.
//  Copyright © 2017 Yizhuang Zhou. All rights reserved.
//

#include <glog/logging.h>

#include <complex>
#include "fft.hpp"
#include "circshift.hpp"

using std::complex;

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


// ******************

cfft2::cfft2() {}
cfft2::~cfft2() {
	dst.release();
	fftwf_destroy_plan(plan);
}
void cfft2::init(InputArray in, const int *n) {
	CHECK_EQ(in.cols(), n[0]*n[1]) << "Size inconsistent";
	CHECK_EQ(in.type(), CV_32F) << "Input type error";
	rows = n[0]; cols = n[1]; // spatial size
	src = in.getMat();
	dst.create(in.rows(), n[0]*(n[1]/2+1), CV_32FC2);
	plan = fftwf_plan_many_dft_r2c(2, n, in.rows(),
								   src.ptr<float>(), NULL, 1, n[0]*n[1],
								   dst.ptr<fftwf_complex>(), NULL, 1, n[0]*(n[1]/2+1),
								   FFTW_ESTIMATE);
}
void cfft2::compute() {
	// perform fftw2
	fftwf_execute(plan);
	// rearrange output
	rearrange();
}

void cfft2::rearrange() {
	for (int i = 0; i < src.rows; i++) {
		// select dst data block
		Mat ds = Mat(rows, cols/2+1, CV_32FC2, dst.row(i).data);

		circshift(ds, ds, ds.cols-1, ds.rows/2);
		Mat lh(ds, Range::all(), Range(0,ds.cols-1));
		rotate(lh, lh, ROTATE_180);
		lh.forEach<complex<float>>([](complex<float> &c, const void*) -> void { c.imag(-c.imag()); });
	}
}

cifft2::cifft2() {}
cifft2::~cifft2() {
	dst.release();
	fftwf_destroy_plan(plan);
}
void cifft2::init(InputArray in, const int *n) {
	CHECK_EQ(in.cols(), n[0]*(n[1]/2+1)) << "Size inconsistent";
	CHECK_EQ(in.type(), CV_32FC2) << "Input type error";
	rows = n[0]; cols = n[1]; // spatial size
	src = in.getMat();
	dst.create(in.rows(), n[0]*n[1], CV_32F);
	plan = fftwf_plan_many_dft_c2r(2, n, in.rows(),
								   src.ptr<fftwf_complex>(), NULL, 1, n[0]*(n[1]/2+1),
								   dst.ptr<float>(), NULL, 1, n[0]*n[1],
								   FFTW_ESTIMATE);
}

void cifft2::compute() {
	// rearrange output
	rearrange();
	// perform fftw2
	fftwf_execute(plan);
}

void cifft2::rearrange() {
	for (int i = 0; i < src.rows; i++) {
		// select dst data block
		Mat ss = Mat(rows, cols/2+1, CV_32FC2, src.row(i).data);
		
		Mat lh(ss, Range::all(), Range(0,ss.cols-1));
		rotate(lh, lh, ROTATE_180);
		lh.forEach<complex<float>>([](complex<float> &c, const void*) -> void { c.imag(-c.imag()); });
		circshift(ss, ss, -(ss.cols-1), -(ss.rows/2));
	}
}

/*
 original:         target:
------------------------------
[0] o ooo [3]    [4] *** o [2]
			         *** o
[1] o ooo [4]        *** o
    o ooo
    o ooo        [3] *** o [0]
 
[2] o ooo [5]    [5] *** o [1]
    o ooo            *** o
    o ooo            *** o
 
 [*] means rotated conj
 */
void cfft2::rearrange2() {
	for (int i = 0; i < src.rows; i++) {
		// select dst data block
		Mat ds = Mat(rows, cols/2+1, CV_32FC2, dst.ptr(i, 0));
		Mat tmp; ds.copyTo(tmp);
		
		Mat s0(tmp, Range(0,1), Range(0,1));
		Mat s1(tmp, Range(1,ds.rows/2+1), Range(0,1));
		Mat s2(tmp, Range(ds.rows/2+1,ds.rows), Range(0,1));
		Mat s3(tmp, Range(0,1), Range(1,ds.cols));
		Mat s4(tmp, Range(1,ds.rows/2+1), Range(1,ds.cols));
		Mat s5(tmp, Range(ds.rows/2+1,ds.rows), Range(1,ds.cols));
		
//		LOG(INFO) << '\n' << s0 << '\n' << s1 << '\n' << s2 << '\n' << s3 << '\n' << s4 << '\n' << s5;
		
		Mat d0(ds, Range(ds.rows/2,ds.rows/2+1), Range(ds.cols-1,ds.cols));
		Mat d1(ds, Range(ds.rows/2+1,ds.rows), Range(ds.cols-1,ds.cols));
		Mat d2(ds, Range(0,ds.rows/2), Range(ds.cols-1,ds.cols));
		Mat d3(ds, Range(ds.rows/2,ds.rows/2+1), Range(0,ds.cols-1));
		Mat d4(ds, Range(0,ds.rows/2), Range(0,ds.cols-1));
		Mat d5(ds, Range(ds.rows/2+1,ds.rows), Range(0,ds.cols-1));
		
		s0.copyTo(d0);
		s1.copyTo(d1);
		s2.copyTo(d2);
		s3.forEach<complex<float>>([](complex<float> &c, const void*) -> void { c.imag(-c.imag()); });
		s4.forEach<complex<float>>([](complex<float> &c, const void*) -> void { c.imag(-c.imag()); });
		s5.forEach<complex<float>>([](complex<float> &c, const void*) -> void { c.imag(-c.imag()); });
		rotate(s3, s3, ROTATE_180);
		rotate(s4, s4, ROTATE_180);
		rotate(s5, s5, ROTATE_180);
		s3.copyTo(d3);
		s4.copyTo(d4);
		s5.copyTo(d5);
	}
}
// ******************


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

//void cfft2(cv::InputArray src, cv::OutputArray dst) {
//	Mat tmp;
//	fft2(src, tmp);
//	fftshift(tmp, dst);
//}
//void cifft2(cv::InputArray src, cv::OutputArray dst) {
//	Mat tmp;
//	ifftshift(src, tmp);
//	ifft2(tmp, dst);
//}
