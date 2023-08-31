#pragma once
#include "metal.h"

struct Refract : Material {
	double ref_idx = 0;
	Refract(double ri) : ref_idx(ri) {}
	bool Scatter(ray const& in, hit_rec const& rec, scatter_rec& srec) const override;
};
