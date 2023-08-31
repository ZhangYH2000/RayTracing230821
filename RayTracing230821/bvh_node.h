#pragma once
#include "hittable.h"

struct BVHNode : Hittable {
	aabb bbox;
	ptr<Hittable> left, right;
	BVHNode(vector<ptr<Hittable>> &obs, int beg, int end);
	BVHNode(vector<ptr<Hittable>> &obs) : BVHNode(obs, 0, obs.size()) {}

	bool Hit(ray const& r, double tmin, double tmax, hit_rec& rec) const override;
	aabb BoundingBox() const override { return bbox; }
};

inline bool box_comp_x(ptr<Hittable> box0, ptr<Hittable> box1) {
	return box0->BoundingBox().x0 < box1->BoundingBox().x0;
}
inline bool box_comp_y(ptr<Hittable> box0, ptr<Hittable> box1) {
	return box0->BoundingBox().y0 < box1->BoundingBox().y0;
}
inline bool box_comp_z(ptr<Hittable> box0, ptr<Hittable> box1) {
	return box0->BoundingBox().z0 < box1->BoundingBox().z0;
}
