#pragma once
#include "hittable.h"

struct HitList : Hittable {
	aabb bbox;
	vector<ptr<Hittable>> obs;
	HitList() {}

	void Add(ptr<Hittable> ob);
	bool Hit(ray const& r, double tmin, double tmax, hit_rec& rec) const override;
	aabb BoundingBox() const override { return bbox; }
};
