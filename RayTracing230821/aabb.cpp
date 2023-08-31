#include "aabb.h"

aabb::aabb(aabb const& box0, aabb const& box1) {
	x0 = min(box0.x0, box1.x0);
	x1 = max(box0.x1, box1.x1);
	y0 = min(box0.y0, box1.y0);
	y1 = max(box0.y1, box1.y1);
	z0 = min(box0.z0, box1.z0);
	z1 = max(box0.z1, box1.z1);
}
bool aabb::hit(ray const& r, double tmin, double tmax) const {
#define TMP(x) \
	t0 = min((x##0 - r.o.x) / r.v.x, (x##1 - r.o.x) / r.v.x); \
	t1 = max((x##0 - r.o.x) / r.v.x, (x##1 - r.o.x) / r.v.x); \
	tmin = max(t0, tmin); tmax = min(t1, tmax); \
	if (tmax < tmin) { return false; }

	// 实践发现上面取严格不等号是重要的，三角形的情形有可能厚度为 0。
	// 有个问题是溢出和除以 0 是怎么解决的。
	// 可能有 0 / 0 的问题，但是在这里不重要。
	double t0 = 0, t1 = 0;
	TMP(x); TMP(y); TMP(z); return true;
#undef TMP
}
