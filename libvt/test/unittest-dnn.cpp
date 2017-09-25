//
//  unittest-dnn.cpp
//  vt
//
//  Created by Yizhuang Zhou on 19/09/2017.
//  Copyright © 2017 Yizhuang Zhou. All rights reserved.
//

#include <stdio.h>

#include "gtest/gtest.h"

#include <glog/logging.h>
#include <opencv2/dnn.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

TEST(DNN, DISABLED_LOAD_NET) {
	dnn::Net net = dnn::readNetFromCaffe("/Users/zhouyz/Development/Cpp/vt/libvt/src/feature/deep/model/deploy.prototxt", "/Users/zhouyz/Development/Cpp/vt/libvt/src/feature/deep/model/squeezenet_v1.0.caffemodel");
	EXPECT_EQ(net.empty(), false);
	Mat im = imread("/Users/zhouyz/Pictures/cat.jpg");
	Mat inputBlob = dnn::blobFromImage(im, 1, Size(256, 256),
								  Scalar(104, 117, 123));
	
	Mat prob;
	TickMeter t;
	net.setInput(inputBlob, "data");
	t.start();
	prob = net.forward("prob");
	t.stop();
	double max;
	Point classnum;
	minMaxLoc(prob.reshape(1,1), NULL, &max, NULL, &classnum);
	LOG(INFO) << classnum.x << ' ' << max;
//	LOG(INFO) << prob.reshape(1,1);
	LOG(INFO) << t.getTimeMilli();
}
