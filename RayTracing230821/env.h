#pragma once
#include "rthm_num.h"

struct Var;
struct rhythm;
struct Inst;
struct Env {
	rthm_num attack, hold, decay, sustain, release;
	rthm_num a_exp, d_exp, r_exp;

	Env(rhythm& ry, Var const& v);
	double val(rhythm& ry, Inst& inst) const;
};
