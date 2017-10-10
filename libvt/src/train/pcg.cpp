//
//  pcg.cpp
//  vt
//
//  Created by Yizhuang Zhou on 09/10/2017.
//  Copyright © 2017 Yizhuang Zhou. All rights reserved.
//

#include "pcg.hpp"

float normSqr(Mat &a, Mat &b) {
	Mat c = a.t() * b;
	return c.at<float>(0);
}

void pcg(const Mat &A, Mat &x, const Mat &b, const Mat &M, int maxit) {
	Mat r = b - A * x;
	Mat p, z;
	float rho0, rho1; // 0-new, 1-old
	float alpha, beta;
	
	for (int i = 0; i < maxit; i++) {
		if (M.empty())
			z = r;
		else
			z = M.mul(r);
		rho0 = normSqr(r, z);
		
		if (i == 0) {
			p = z;
		} else {
			beta = rho0 / rho1;
			p = z + beta * p;
		}
		
		Mat Ap = A * p;
		alpha = rho0 / normSqr(p, Ap);
		x = x + alpha * p;
		r = r - alpha * Ap;
		
		if (sqrt(rho0) < 1e-9)
			break;

		rho1 = rho0;
	}
}

//template <typename Functor>
//void fpcg(const Functor &A, Mat &x, const Mat &b, const Mat &M, int maxit) {
//	Mat r = b - A(x);
//	Mat p, z;
//	float rho0, rho1; // 0-new, 1-old
//	float alpha, beta;
//	
//	for (int i = 0; i < maxit; i++) {
//		if (M.empty())
//			z = r;
//		else
//			z = M.mul(r);
//		rho0 = normSqr(r, z);
//		
//		if (i == 0) {
//			p = z;
//		} else {
//			beta = rho0 / rho1;
//			p = z + beta * p;
//		}
//		
//		Mat Ap = A(p);
//		alpha = rho0 / normSqr(p, Ap);
//		x = x + alpha * p;
//		r = r - alpha * Ap;
//		
//		if (sqrt(rho0) < 1e-9)
//			break;
//		
//		rho1 = rho0;
//	}
//}

void fpcg(function<Mat(Mat&)> A,Mat &x, const Mat &b, const Mat &M, int maxit) {
	Mat r = b - A(x);
	Mat p, z;
	float rho0, rho1; // 0-new, 1-old
	float alpha, beta;
	
	for (int i = 0; i < maxit; i++) {
		if (M.empty())
			z = r;
		else
			z = M.mul(r);
		rho0 = normSqr(r, z);
		
		if (i == 0) {
			p = z;
		} else {
			beta = rho0 / rho1;
			p = z + beta * p;
		}
		
		Mat Ap = A(p);
		alpha = rho0 / normSqr(p, Ap);
		x = x + alpha * p;
		r = r - alpha * Ap;
		
		if (sqrt(rho0) < 1e-9)
			break;
		
		rho1 = rho0;
	}
}
