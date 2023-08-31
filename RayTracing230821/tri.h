#pragma once
#include "geo.h"
#include "hittable.h"

struct Material;
struct Tri : Hittable {
	tri t;
	aabb bbox;
	ptr<Material> mat;

	Tri(tri const &t, ptr<Material> m);
	bool Hit(ray const& r, double tmin, double tmax, hit_rec& rec) const override;
	aabb BoundingBox() const override { return bbox; }
};
