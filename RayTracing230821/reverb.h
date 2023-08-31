#pragma once
#include "rthm_num.h"

struct Comb {
	deque<double> vs;
	Comb(int n = 0);
	void reset();
	double loop(double in, double g);
};

struct AllPass {
	deque<double> vs, ins;
	AllPass(int n = 0);
	void reset();
	double loop(double in, double g);
};

struct Var;
struct rhythm;
struct Filter;
struct Reverb {
	rthm_num wet, gc0, gc1, gc2, gc3, ga0, ga1;
	Comb c0, c1, c2, c3;
	AllPass a0, a1;
	vector<ptr<Filter>> fs;

	Reverb(rhythm &ry, Var const& v);
	void reset(rhythm& ry);
	double loop(rhythm& ry, double in);
};
