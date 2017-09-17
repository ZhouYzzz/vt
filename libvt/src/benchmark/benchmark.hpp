//
//  benchmark.hpp
//  vt
//
//  Created by Yizhuang Zhou on 17/09/2017.
//  Copyright © 2017 Yizhuang Zhou. All rights reserved.
//

#ifndef benchmark_hpp
#define benchmark_hpp

#include <stdio.h>
#include <dirent.h>
#include <iostream>
#include <vector>
#include <fstream>

#include <glog/logging.h>
#include <opencv2/core.hpp>

#include "sequence.hpp"

class Benchmark {
protected:
	std::string root_dir_;
	std::vector<sequence*> sequences_;
	
public:
	Benchmark() {}
	~Benchmark() {}
	
	std::vector<sequence*> sequences() { return sequences_; }
	
};

class OTB : public Benchmark {
public:
	OTB() {}
	~OTB() {}
	OTB(std::string root_dir);
	
	static sequence * construct(std::string root_dir, std::string seq_name);
};

#endif /* benchmark_hpp */
