//
//  unittest-benchmark.cpp
//  vt
//
//  Created by Yizhuang Zhou on 17/09/2017.
//  Copyright © 2017 Yizhuang Zhou. All rights reserved.
//

#include <stdio.h>

#include "gtest/gtest.h"

#include "benchmark.hpp"

TEST(Benchmark, DISABLED_Benchmark) {
	OTB b = OTB("/Users/zhouyz/Development/Data/OTB50");
	EXPECT_EQ(b.sequences().size(), 49); // no Human50
}

TEST(Benchmark, StaticMethod) {
	auto s = OTB::construct("/Users/zhouyz/Development/Data/OTB50", "Bolt");
	EXPECT_EQ(s->end_frame - s->start_frame, 350);
	EXPECT_EQ(s->ground_truth_rects.size(), 350);
	EXPECT_EQ(s->image_files.size(), 350);
	auto r = s->ground_truth_rects[0];
	// cv::Rect2f(336,165,26,61)
	EXPECT_FLOAT_EQ(r.x, 336);
	EXPECT_FLOAT_EQ(r.y, 165);
	EXPECT_FLOAT_EQ(r.width, 26);
	EXPECT_FLOAT_EQ(r.height, 61);
	EXPECT_EQ(s->image_files[0], "/Users/zhouyz/Development/Data/OTB50/Bolt/img/0001.jpg");
}

TEST(Benchmark, Sequence) {
	auto s = OTB::construct("/Users/zhouyz/Development/Data/OTB50", "Bolt");
	cv::Mat image = s->get_next_frame();
	EXPECT_EQ(image.rows, 360); // height
	EXPECT_EQ(image.cols, 640); // width
	EXPECT_EQ(image.channels(), 3); // BGR channels
}
