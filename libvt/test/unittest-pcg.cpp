//
//  unittest-pcg.cpp
//  vt
//
//  Created by Yizhuang Zhou on 09/10/2017.
//  Copyright © 2017 Yizhuang Zhou. All rights reserved.
//

#include <stdio.h>

#include "gtest/gtest.h"

#include "pcg.hpp"

TEST(PCG, 0) {
	Mat A = Mat(2, 2, CV_32F);
	A.at<float>(0,0) = 4;
	A.at<float>(0,1) = 1;
	A.at<float>(1,0) = 1;
	A.at<float>(1,1) = 3;
	Mat x = Mat(2,1, CV_32F);
	x.at<float>(0,0) = 2;
	x.at<float>(1,0) = 1;
	Mat b = Mat(2,1, CV_32F);
	b.at<float>(0,0) = 1;
	b.at<float>(1,0) = 2;
	Mat M = Mat(2,1, CV_32F);
	M.at<float>(0,0) = 1;
	M.at<float>(1,0) = 1;
	pcg(A, x, b, M, 20);
	LOG(INFO) << x;
	LOG(INFO) << A * x;
	
//	struct lhs {
//		Mat A_;
//		Mat operator()(Mat &x) { return A_ * x; }
//	} Af;
//	Af.A_ = A;
//	FA fa;
//	fa.A = A;
	x.at<float>(0,0) = 0;
	x.at<float>(1,0) = 0;
	
	auto funA = [&A](Mat &x)->Mat {
		return A * x;
	};
	fpcg(funA, x, b, M, 20);
//	fpcg(A, x, b, M, 20);
	LOG(INFO) << x;
	LOG(INFO) << funA(x);
}
