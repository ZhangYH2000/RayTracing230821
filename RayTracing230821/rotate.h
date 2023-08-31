#pragma once
#include "hittable.h"

struct Rotate : Hittable {
	mat3 tsf, inv_tsf;
	ptr<Hittable> ob;

	Rotate(ptr<Hittable> ob, mat3 const &tsf) : ob(ob), tsf(tsf), inv_tsf(tsf.inv()) {}
	bool Hit(ray const& r, double tmin, double tmax, hit_rec& rec) const override;
	aabb BoundingBox() const override { return {}; };
	double PDFVal(vec3 o, vec3 v) const override;
	vec3 Rnd(vec3 o) const override;
};
