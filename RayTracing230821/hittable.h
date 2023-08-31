#pragma once
#include "aabb.h"

struct Material;
struct hit_rec {
	vec2 u;
	vec3 p, n;
	double t = 0;
	ptr<Material> m;
	bool front = false;

	void deal_front(ray const& r, vec3 n_out) {
		front = dot(r.v, n_out) < 0;
		n = front ? n_out : -n_out;
	}
};

struct aabb;
struct Hittable {
	virtual ~Hittable() {}
	virtual bool Hit(ray const& r, double tmin, double tmax, hit_rec& rec) const = 0;
	virtual aabb BoundingBox() const = 0;
	virtual double PDFVal(vec3 o, vec3 v) const { return 0; }
	virtual vec3 Rnd(vec3 o) const { return {}; }
};
