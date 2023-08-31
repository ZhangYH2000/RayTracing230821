#pragma once
#include "utils.h"

int constexpr TYP_NUM = 0;
int constexpr TYP_PAR = 1;
int constexpr TYP_ENV = 2;
int constexpr TYP_LFO = 3;

struct Var;
struct rhythm;
struct Inst;
struct rthm_num {
	int type = 0;
	double num = 0;
	double a = 0, b = 0;
	int id = 0;

	rthm_num(double num = 0);
	rthm_num(rhythm& ry, Var const& v);
	double val(rhythm& ry) const;
	double val(rhythm& ry, Inst& inst) const;
};
