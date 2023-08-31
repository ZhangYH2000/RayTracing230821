#pragma once
#include "hittable.h"

struct Translate : Hittable {
	vec3 offset;
	ptr<Hittable> ob;

	Translate(ptr<Hittable> ob, vec3 offset) : ob(ob), offset(offset) {}
	bool Hit(ray const& r, double tmin, double tmax, hit_rec& rec) const override;
	aabb BoundingBox() const override { return {}; };
	double PDFVal(vec3 o, vec3 v) const override;
	vec3 Rnd(vec3 o) const override;
};
