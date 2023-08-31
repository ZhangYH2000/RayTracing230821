#pragma once
#include "material.h"

struct Glass : Material {
	col3 col;
	double fuzz = 0;
	Glass(col3 c, double fuzz = 0) : col(c), fuzz(fuzz) {}
	bool Scatter(ray const& in, hit_rec const& rec, scatter_rec& srec) const override;
};
