#pragma once
#include "material.h"

struct mat_item {
	ptr<Material> mat;
	double p = 0;
};

struct Mix : Material {
	vector<mat_item> ms;
	Mix(vector<mat_item> const& ms) : ms(ms) {}
	Mix(ptr<Material> m0, ptr<Material> m1, double p0);

	bool Scatter(ray const& in, hit_rec const& rec, scatter_rec& srec) const override;
	col3 Emit(vec2 u, vec3 p) const override;
};
