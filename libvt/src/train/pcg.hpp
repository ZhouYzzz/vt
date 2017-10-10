//
//  pcg.hpp
//  vt
//
//  Created by Yizhuang Zhou on 09/10/2017.
//  Copyright © 2017 Yizhuang Zhou. All rights reserved.
//

#ifndef pcg_hpp
#define pcg_hpp

#include <stdio.h>

#include <opencv2/core.hpp>
#include <functional>

#include "common.hpp"

using namespace cv;
using std::function;

struct lhs_params {
	Mat A;
};

//void pcg(const Mat &A, Mat &x, const Mat &b, int maxit);
void pcg(const Mat &A, Mat &x, const Mat &b, const Mat &M, int maxit);

//template <typename Functor>
//void fpcg(const Functor &A, Mat &x, const Mat &b, const Mat &M, int maxit);

void fpcg(function<Mat(Mat&)> A,Mat &x, const Mat &b, const Mat &M, int maxit);
#endif /* pcg_hpp */
