//
//  unittest-feature.cpp
//  vt
//
//  Created by Yizhuang Zhou on 17/09/2017.
//  Copyright © 2017 Yizhuang Zhou. All rights reserved.
//

#include <stdio.h>

#include "gtest/gtest.h"

#include <glog/logging.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include "fhog.hpp"
//#include "feature.hpp"
#include "hog.hpp"
#include "deep.hpp"
#include "hann.hpp"


TEST(Feature, DISABLED_Hann) {
	cv::Mat window = hann(cv::Size2i(5,7));
	LOG(INFO) << window;
}

TEST(Feature, DISABLED_MAT) {
	int d[] = {0,1,2,3,4,5};
	cv::Mat m = cv::Mat(cv::Size(3,2), CV_32S, d);
	LOG(INFO) << m;
}

TEST(Feature, DISABLED_Hog) {
	Hog h = Hog();
	cv::Mat image = cv::imread("/Users/zhouyz/Development/Data/OTB50/Bolt/img/0001.jpg");
//	h.reshape(image.size());
	image.convertTo(h.input(), CV_32FC3);
	h.compute();
}

TEST(Feature, DISABLED_TEST_FHOG) {
	cv::Mat image = cv::imread("/Users/zhouyz/Development/Data/OTB50/Bolt/img/0001.jpg");
	IplImage image_ipl = image;
	CvLSVMFeatureMapCaskade *map;
	getFeatureMaps(&image_ipl, 4, &map);
//	normalizeAndTruncate(map, 0.2f);
	PCAFeatureMaps(map);
	LOG(INFO) << map->sizeX << ' ' << map->sizeY << ' ' << map->numFeatures;
	cv::Mat feature = cv::Mat(cv::Size(map->numFeatures, map->sizeX*map->sizeY), CV_32F, map->map);
	LOG(INFO) << feature.cols << ' ' << feature.rows;
	cv::transpose(feature, feature);
	cv::Mat singleChannel = cv::Mat(cv::Size(map->sizeX,map->sizeY), CV_32F, feature.ptr());
	
	cv::Mat planes[] = {singleChannel, cv::Mat1f::zeros(singleChannel.size())};
	
	cv::Mat xf;
	cv::merge(planes, 2, xf);
	cv::dft(xf, xf);
	
	cv::split(xf, planes);
	
	planes[0].copyTo(singleChannel);
	
	double min, max;
	cv::minMaxLoc(singleChannel, &min, &max);
	singleChannel = (singleChannel - min) / (max - min);
//	cv::imshow("imagesc", singleChannel);
//	cv::waitKey();
	freeFeatureMapObject(&map);
}
