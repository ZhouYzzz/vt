//
//  circshift.hpp
//  vt
//
//  Created by Yizhuang Zhou on 21/09/2017.
//  Copyright © 2017 Yizhuang Zhou. All rights reserved.
//

#ifndef circshift_hpp
#define circshift_hpp

#include <stdio.h>

#include <opencv2/core.hpp>

template <typename T>
void circshift(cv::Mat in, cv::Mat out, int xshift, int yshift) {
	int xdim = in.cols;
	int ydim = in.rows;
	
	cv::Mat * src;
	cv::Mat tmp;
	
	if (in.ptr() == out.ptr()) {
		in.copyTo(tmp);
		src = &tmp;
	} else {
		src = &in;
	}

	for (int i = 0; i < xdim; i++) {
		int ii = (i + xshift) % xdim;
		if (ii < 0) ii = xdim + ii;
		for (int j = 0; j < ydim; j++) {
			int jj = (j + yshift) % ydim;
			if (jj < 0) jj = ydim + jj;
			out.at<T>(jj, ii) = src->at<T>(j, i);
		}
	}
}

template <typename T>
inline void fftshift(cv::Mat in, cv::Mat out) {
	circshift<T>(in, out, in.cols/2, in.rows/2);
}

template <typename T>
inline void ifftshift(cv::Mat in, cv::Mat out) {
	circshift<T>(in, out, (in.cols+1)/2, (in.rows+1)/2);
}

#endif /* circshift_hpp */
