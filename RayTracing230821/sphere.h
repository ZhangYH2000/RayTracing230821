#pragma once
#include "hittable.h"

struct Var;
struct Material;
struct Sphere : Hittable {
	vec3 ct;
	aabb bbox;
	double rad = 0;
	ptr<Material> mat;

	Sphere(Var const &v);
	Sphere(vec3 ct, double r, ptr<Material> m);
	bool Hit(ray const& r, double tmin, double tmax, hit_rec& rec) const override;
	aabb BoundingBox() const override { return bbox; }
	double PDFVal(vec3 o, vec3 v) const override;
	vec3 Rnd(vec3 o) const override;
};
