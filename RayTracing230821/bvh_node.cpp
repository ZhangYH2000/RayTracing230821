#include "bvh_node.h"

BVHNode::BVHNode(vector<ptr<Hittable>>& obs, int beg, int end) {
	int axis = drnd(3);
	auto comp =
		axis == 0 ? box_comp_x :
		axis == 1 ? box_comp_y : box_comp_z;
	
	// 默认了元素个数大于等于 2。
	sort(obs.begin() + beg, obs.begin() + end, comp);
	int mid = beg + (end - beg) / 2;
	left = (mid - beg == 1) ? obs[beg] : msh<BVHNode>(obs, beg, mid);
	right = (end - mid == 1) ? obs[mid] : msh<BVHNode>(obs, mid, end);
	bbox = aabb(left->BoundingBox(), right->BoundingBox());
}
bool BVHNode::Hit(ray const& r, double tmin, double tmax, hit_rec& rec) const {
	if (!bbox.hit(r, tmin, tmax)) { return false; }

	hit_rec tmp;
	bool hit = false;
	if (left->Hit(r, tmin, tmax, tmp)) {
		hit = true; tmax = tmp.t;
		rec = tmp;
	}
	if (right->Hit(r, tmin, tmax, tmp)) {
		hit = true; rec = tmp;
	} return hit;
}
