#pragma once
#include "material.h"
#include "col.h"

struct Metal : Material {
	ptr<Texture> t;
	double fuzz = 0;
	Metal(ptr<Texture> t, double f = 0) : t(t), fuzz(f) {}
	bool Scatter(ray const& in, hit_rec const& rec, scatter_rec& srec) const override;
};
