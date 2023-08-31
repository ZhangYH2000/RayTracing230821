#pragma once
#include "ray.h"

struct aabb {
	double x0 = 0, x1 = 0, y0 = 0, y1 = 0, z0 = 0, z1 = 0;
	aabb() = default;
	aabb(double x0, double x1, double y0, double y1, double z0, double z1) :
		x0(x0), x1(x1), y0(y0), y1(y1), z0(z0), z1(z1) {}
	aabb(aabb const& box0, aabb const& box1);

	bool hit(ray const& r, double tmin, double tmax) const;
};
