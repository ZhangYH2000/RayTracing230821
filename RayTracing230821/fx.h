#pragma once
#include "utils.h"

// �Ҳ�֪����� FX_NONE ������������Ժ�Ҫ�ĵ���
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
