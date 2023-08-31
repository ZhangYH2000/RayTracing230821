#pragma once
#include "hittable.h"

struct Var;
struct Model : Hittable {
	ptr<Hittable> ts;
	Model(wstring const& nm);
	Model(Var const &v);
	bool Hit(ray const& r, double tmin, double tmax, hit_rec& rec) const override {
		return ts->Hit(r, tmin, tmax, rec);
	}
	aabb BoundingBox() const override { return ts->BoundingBox(); }
};
