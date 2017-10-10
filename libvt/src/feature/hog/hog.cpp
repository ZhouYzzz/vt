//
//  hog.cpp
//  vt
//
//  Created by Yizhuang Zhou on 19/09/2017.
//  Copyright © 2017 Yizhuang Zhou. All rights reserved.
//

#include "hog.hpp"

#include "fhog.hpp"
#include "hann.hpp"
#include <glog/logging.h>
//#include "imagesc.hpp"

Hog::Hog(cv::Size2i i_sz) {
		n_out_ = 1;
		input_.create(i_sz, CV_32FC3);
		cv::Size2i o_sz;
		o_sz.width = i_sz.width / 4;
		o_sz.height = i_sz.height / 4;

//		int sz[] = { o_sz.height, o_sz.width };
//		cv::MatSize msz(sz); // matsize is { H, W } order;
		out_size_.push_back(o_sz);
		
		int step_ = o_sz.width * o_sz.height;
		float* data_ = static_cast<float*>(cv::fastMalloc(sizeof(float) * step_ * 31)); // allocate output memory
//		for (int nc = 0; nc < 31; nc++) {
//			cv::Mat r = cv::Mat(output_sz, CV_32F, data_);
//			data_ += step_;
//			//			cv::Mat i = cv::Mat(output_sz, CV_32F, data_);
//			//			data_ += step_;
//			output_.push_back(r);
//			//			output().push_back(i);
//		} // construct output vector
		cv::Mat r = cv::Mat(31, step_, CV_32F, data_);
		out_.push_back(data_);
		output_.push_back(r);
		out_channel_.push_back(31);

		cv::Mat window = hann(o_sz);
		hann_.push_back(window);
	}

void Hog::compute() {
	IplImage ipl = input_;
	CvLSVMFeatureMapCaskade * map;
	getFeatureMaps(&ipl, 4, &map);
	PCAFeatureMaps(map);
	
	cv::Mat feature = cv::Mat(map->sizeX * map->sizeY, map->numFeatures, CV_32F, map->map);
	cv::transpose(feature, output_[0]);
	
//	cv::Mat feature = cv::Mat(cv::Size(map->sizeX, map->sizeY), CV_32FC(31), map->map);
//	cv::split(feature, output_);

	// multiply with hanning window
	
//	int step_ = map->sizeX * map->sizeY;
//	float * tmp_ptr_ = out_[0];
	for (int i = 0; i < map->numFeatures; i++) {
		cv::Mat selected_mat = cv::Mat(map->sizeY, map->sizeX, CV_32F, output_[0].ptr<float>(i, 0));
		cv::multiply(selected_mat, hann_[0], selected_mat);
	}
	freeFeatureMapObject(&map);
}

// **************************

HogFeature::HogFeature() {}
HogFeature::~HogFeature() {
	src.release();
	dst.release();
}

HogFeature::HogFeature(Size2i input_size, hog_parameter param) {
	src.create(input_size.height, input_size.width, CV_32FC3);
	output_sz.height = input_size.height / 4;
	output_sz.width = input_size.width / 4;
	dst.create(31, output_sz.height * output_sz.width, CV_32F);
//	win = Mat(1,output_sz.height*output_sz.width,CV_32F,hann(output_sz).data);
	win = hann(output_sz);
}

void HogFeature::compute() {
	IplImage ipl = src;
	CvLSVMFeatureMapCaskade *map;
	getFeatureMaps(&ipl, 4, &map);
	PCAFeatureMaps(map);
	Mat feature = Mat(map->sizeX * map->sizeY, map->numFeatures, CV_32F, map->map);
	transpose(feature, dst);
	
//	for (int i = 0; i < map->numFeatures; i++) {
//		Mat row = dst.row(i);
//		multiply(row, win, row);
//	}
	
	for (int i = 0; i < map->numFeatures; i++) {
		Mat selected_mat = cv::Mat(map->sizeY, map->sizeX, CV_32F, dst.row(i).data);
		multiply(selected_mat, win, selected_mat);
	}
	
	freeFeatureMapObject(&map);
}
















