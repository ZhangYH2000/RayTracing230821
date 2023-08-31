#pragma once
#include "pass.h"
#include "hittable.h"

struct Material;
struct Medium : Hittable {
	ptr<Hittable> shape;
	ptr<Material> phase, border, pass;
	double neg_inv_density = 0;

	Medium(ptr<Hittable> s, ptr<Material> p, ptr<Material> b, double d) : 
		shape(s), phase(p), border(b), pass(msh<Pass>()), neg_inv_density(-1 / d) {}
	bool Hit(ray const& r, double tmin, double tmax, hit_rec& rec) const override;
	aabb BoundingBox() const override { return shape->BoundingBox(); }
};
