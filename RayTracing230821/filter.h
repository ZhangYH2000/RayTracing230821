#pragma once
#include "rthm_num.h"

int constexpr FLT_LP = 0;
int constexpr FLT_HP = 1;
int constexpr FLT_BP = 2;
int constexpr FLT_ALT_LP = 3;
int constexpr FLT_ALT_HP = 4;

struct Var;
struct rhythm;
struct Inst;
struct Filter {
	// Paull Kellett 滤波器，我还不知道具体常数的选取原因。
	int type = 0;
	rthm_num freq, resonance;
	double buf0 = 0, buf1 = 0;

	Filter(rhythm& ry, Var const& v);
	double loop(rhythm& ry, double in);
	double loop(rhythm& ry, Inst& inst, double in);
};
