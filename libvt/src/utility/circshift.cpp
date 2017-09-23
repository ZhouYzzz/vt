//
//  circshift.cpp
//  vt
//
//  Created by Yizhuang Zhou on 21/09/2017.
//  Copyright © 2017 Yizhuang Zhou. All rights reserved.
//

#include "circshift.hpp"

using namespace cv;

void circshift(InputArray src, OutputArray dst, int xshift, int yshift) {
	int xd = src.cols(); // dim
	int yd = src.rows();
	int xs = xshift % xd; // split point
	int ys = yshift % yd;
	if (xs < 0) xs += xd;
	if (ys < 0) ys += yd;
	
	Mat srcMat;
	if (src.getObj() == dst.getObj()) {
		src.copyTo(srcMat);
	} else {
		dst.createSameSize(src, src.type());
		srcMat = src.getMat();
	}
	Mat & dstMat = dst.getMatRef();
	
	Mat b1(dstMat, Range(0,ys), Range(xs,xd));
	Mat b2(dstMat, Range(0,ys), Range(0,xs));
	Mat b3(dstMat, Range(ys,yd), Range(0,xs));
	Mat b4(dstMat, Range(ys,yd), Range(xs,xd));
	
	Mat c1(srcMat, Range(yd-ys,yd), Range(0,xd-xs));
	Mat c2(srcMat, Range(yd-ys,yd), Range(xd-xs,xd));
	Mat c3(srcMat, Range(0,yd-ys), Range(xd-xs,xd));
	Mat c4(srcMat, Range(0,yd-ys), Range(0,xd-xs));
	
	c1.copyTo(b1);
	c2.copyTo(b2);
	c3.copyTo(b3);
	c4.copyTo(b4);
}

void fftshift(InputArray src, OutputArray dst) { circshift(src, dst, src.cols()/2, src.rows()/2); }
void ifftshift(InputArray src, OutputArray dst) { circshift(src, dst, (src.cols()+1)/2, (src.rows()+1)/2); }
void fftshift(cv::InputOutputArray src) { fftshift(src, src); }
void ifftshift(cv::InputOutputArray src) { ifftshift(src, src); }
