#pragma once
#include "utils.h"

struct nrect {
	int tone = 0;
	double t0 = 0, t1 = 0;
	nrect(FILE* f);
};
