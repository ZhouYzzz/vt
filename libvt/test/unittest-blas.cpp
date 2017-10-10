////
////  unittest-blas.cpp
////  vt
////
////  Created by Yizhuang Zhou on 06/10/2017.
////  Copyright © 2017 Yizhuang Zhou. All rights reserved.
////
//
//#include <stdio.h>
//
//#include "gtest/gtest.h"
//
//#include <glog/logging.h>
//#include <Accelerate/Accelerate.h>
//#include <opencv2/core.hpp>
//#include <opencv2/imgcodecs.hpp>
//#include <opencv2/imgproc.hpp>
//#include "fourier_coefficients.hpp"
//#include "fft.hpp"
//#include "circshift.hpp"
//#include "hog.hpp"
//#include "imagesc.hpp"
//
//TEST(BLAS, BLAS) {
////	cblas_dgemm(CBLAS_ROW_MAJOR, <#const enum CBLAS_TRANSPOSE __TransA#>, <#const enum CBLAS_TRANSPOSE __TransB#>, <#const int __M#>, <#const int __N#>, <#const int __K#>, <#const double __alpha#>, <#const double *__A#>, <#const int __lda#>, <#const double *__B#>, <#const int __ldb#>, <#const double __beta#>, <#double *__C#>, <#const int __ldc#>);
////	_mulx_u64(<#unsigned long long __X#>, <#unsigned long long __Y#>, <#unsigned long long *__P#>)
////	cblas_ddot(<#const int __N#>, <#const double *__X#>, <#const int __incX#>, <#const double *__Y#>, <#const int __incY#>);
//}
//
//TEST(BLAS, PCA) {
//	using namespace cv;
//	float in[] = {0,1,2,3,4,5,6,7,8};
//	Mat A = cv::Mat(3,3,CV_32F,in);
//	LOG(INFO) << A;
//	Mat m, e, R;
//	PCACompute(A, m, e, 2);
//	PCAProject(A, m, e, R);
//	LOG(INFO) << m;
//	LOG(INFO) << e;
//	LOG(INFO) << R;
//	PCA p = PCA(A, m, PCA::DATA_AS_ROW, 2);
//	LOG(INFO) << p.project(A);
//}
//
//TEST(BLAS, FULL) {
//	auto t = cv::TickMeter();
//
//	float in[] = {0,2,2,2,4,4,4,6,8,0,0,0,1,1,1};
//	Mat A = cv::Mat(1,15,CV_32F, in);
//	Mat Af;
//	t.start();
////	for (int i = 0; i < 1000; i++) {
//		fft2(A, Af);
//		fftshift(Af);
////	}
//	t.stop();
//	LOG(INFO) << "raw " << t.getTimeMilli() << " ms";
//	t.reset();
////	LOG(INFO) << '\n' << Af;
//	
////	Mat B = Mat(1, 51*51, CV_32F, Scalar(0));
////	int n[] = {51,51};
//	int n[] = {3,5};
//	auto f = cfft2(A, n);
//	
//	t.start();
//	
////	for (int i = 0; i < 1000; i++)
//		f.compute();
//	LOG(INFO) << f.dst;
//	
//	t.stop();
//	LOG(INFO) << "fftw " << t.getTimeMilli() << " ms";
//	
//	auto fi = cifft2(f.dst, n);
//	
//	fi.compute();
//	LOG(INFO) << fi.dst;
//	
////	t.reset();
////	t.start();
//////	for (int i = 0; i < 1000; i++)
////		f.rearrange();
////	LOG(INFO) << f.dst;
////
////	t.stop();
////	LOG(INFO) << "rearr " << t.getTimeMilli() << " ms";
////	
////	f.compute();
////	f.rearrange2();
//	
////	LOG(INFO) << f.dst;
////	LOG(INFO) << '\n' << f.dst;
////
//////	Mat Afc;
////	compact_fourier_coefficients(Af, Af);
////	LOG(INFO) << '\n' << Af;
////	
//////	Mat Af2;
////	full_fourier_coefficients(Af, Af);
////	LOG(INFO) << '\n' << Af;
//}
//
//TEST(BLAS, DISABLED_Speedfft) {
//	auto t = cv::TickMeter();
//
//	Mat A = cv::Mat(5,51*51,CV_32F, Scalar(1));
//	int n[] = {51,51};
//	auto f = cfft2(A,n);
//	
//	t.start();
//	for (int i = 0; i < 100; i++)
//		f.compute();
//	t.stop();
//	LOG(INFO) << t.getTimeMilli();
//	t.reset();
//	t.start();
//	for (int i = 0; i < 100; i++)
//		f.rearrange();
//	t.stop();
//	LOG(INFO) << t.getTimeMilli();
//	t.reset();
//	t.start();
//	for (int i = 0; i < 100; i++)
//		f.rearrange2();
//	t.stop();
//	LOG(INFO) << t.getTimeMilli();
//
//}
//
//TEST(HogFeature, h) {
//	Mat image = imread("/Users/zhouyz/Pictures/cat.jpg");
//	resize(image, image, Size2i(128,128));
//	hog_parameter pa;
//	auto h = HogFeature(Size2i(128,128), pa);
//	image.convertTo(h.input(), CV_32FC3);
//	h.compute();
//	
//	Mat mean;
//	
//	auto p = PCA(h.output(), mean, PCA::DATA_AS_COL, 10);
//	
//	Mat r;
//	p.project(h.output(), r);
//	
////	Mat d = Mat(32, 32, CV_32F, r.row(2).data);
////	imagesc("hog", d);
//}
