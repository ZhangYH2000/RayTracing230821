#pragma once
#include "rthm_num.h"

int constexpr LFO_SIN = 0;
int constexpr LFO_SQR = 1;
int constexpr LFO_TRI = 2;
int constexpr LFO_SAW = 3;

struct Var;
struct rhythm;
struct Inst;
struct LFO {
	rthm_num freq;
	int type = 0;
	bool user_shape = false;
	wstring nm_fun;
	bool sheet_time = false;

	LFO(rhythm &ry, Var const& v);
	double val(rhythm &ry, Inst& inst) const;
};
