#pragma once
#include "vec3.h"

struct ray {
	int in = 0;
	vec3 o, v;
	ray() = default;
	ray(vec3 o, vec3 v, int in = 0) : o(o), v(v), in(in) {}
	vec3 at(double t) const { return o + v * t; }
};

struct Cur;
struct Hittable;
col3 ray_col(ray const& r, int dep, Cur &cur);
