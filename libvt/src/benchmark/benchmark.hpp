//
//  benchmark.hpp
//  vt
//
//  Created by Yizhuang Zhou on 17/09/2017.
//  Copyright © 2017 Yizhuang Zhou. All rights reserved.
//

#ifndef BENCHMARK_HPP_
#define BENCHMARK_HPP_

#include <iostream>
#include <vector>

#include "common.hpp"
#include "sequence.hpp"


class Benchmark {
protected:
	string root_dir_;
	vector<sequence*> sequences_;
	
public:
	Benchmark() {}
	~Benchmark() {}
	
	vector<sequence*> sequences() { return sequences_; }
	
};

class OTB : public Benchmark {
public:
	OTB() {}
	~OTB() {}
	OTB(string root_dir);
	
	static sequence * construct(string root_dir, string seq_name);
};

class VOT : public Benchmark {
public:
	VOT() {}
	~VOT() {}
	VOT(string root_dir);

	static sequence * construct(string root_dir, string seq_name);
};


#endif /* BENCHMARK_HPP_ */
