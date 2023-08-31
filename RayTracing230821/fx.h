#pragma once
#include "utils.h"

// 我不知道这个 FX_NONE 的意义在哪里，以后要改掉。
int constexpr FX_NONE = 0;
int constexpr FX_REVERB = 1;

struct Var;
struct rhythm;
struct Reverb;
struct FX {
	int type = 0;
	ptr<Reverb> reverb;

	FX(rhythm &ry, Var const& v);
	void reset(rhythm& ry);
	double loop(rhythm& ry, double in);
};
