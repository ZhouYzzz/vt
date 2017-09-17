//
//  benchmark.cpp
//  vt
//
//  Created by Yizhuang Zhou on 17/09/2017.
//  Copyright © 2017 Yizhuang Zhou. All rights reserved.
//

#include "benchmark.hpp"

OTB::OTB(std::string root_dir) {
	root_dir_ = root_dir;
	DIR * pdir = opendir(root_dir_.data());
	if (pdir == NULL)
		LOG(FATAL) << "Cannot open dir " << root_dir_;
	struct dirent * pent;
	pent = readdir(pdir); // read '.' dir
	pent = readdir(pdir); // read '..' dir
	while ((pent = readdir(pdir))) {
		if (pent->d_type == DT_DIR) {
			auto s = construct(root_dir_, pent->d_name);
			if (s) sequences_.push_back(s);
		}
	}
	closedir(pdir);
}

sequence * OTB::construct(std::string root_dir, std::string seq_name) {
	// Check existence of sequence dir
	std::string seq_dir = root_dir + '/' + seq_name;
	DIR * pdir = opendir(seq_dir.data());
	if (pdir == NULL)
		LOG(FATAL) << "Cannot open dir " << seq_dir;
	closedir(pdir);
	
	// Create sequence
	sequence * s = new sequence;
	s->name = seq_name;
	
	// Handle Special Case
	if (!std::strcmp(seq_name.data(), "Human4"))
		return NULL; // there's something wrong with Human4
	
	// Parse groundtruth file
	std::string groundtruth_file_path = seq_dir + '/' + "groundtruth_rect.txt";
	std::ifstream groundtruth_file(groundtruth_file_path, std::ios::in);
	if (!groundtruth_file.is_open())
		LOG(FATAL) << "Missing groundtruth_rect.txt";
	std::string line;
	while (std::getline(groundtruth_file, line)) {
		float x,y,w,h;
		std::stringstream ss(line);
		ss >> x; ss.ignore();
		ss >> y; ss.ignore();
		ss >> w; ss.ignore();
		ss >> h;
		cv::Rect2f r = cv::Rect2f(x,y,w,h);
		s->ground_truth_rects.push_back(r);
	}
	s->end_frame += s->ground_truth_rects.size();
	
	// Handle special case
	if (!std::strcmp(seq_name.data(), "David")) {
		s->start_frame += 300 - 1; // start from 300
		s->end_frame += 300 - 1;
		s->current_frame = s->start_frame;
	}
	
	// Add image file names
	std::string fmt_str = seq_dir + "/img/%04d.jpg";
	char buffer_[fmt_str.size() + 20];
	for (int is = s->start_frame; is < s->end_frame; is++) {
		sprintf(buffer_, fmt_str.data(), is + 1); // start from 1
		s->image_files.push_back(buffer_);
	}
	
	return s;
}
