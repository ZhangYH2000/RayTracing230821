#include "tri.h"

Tri::Tri(tri const& t, ptr<Material> m) : t(t), mat(m) {
	auto x0 = min(min(t.va.x, t.vb.x), t.vc.x);
	auto x1 = max(max(t.va.x, t.vb.x), t.vc.x);
	auto y0 = min(min(t.va.y, t.vb.y), t.vc.y);
	auto y1 = max(max(t.va.y, t.vb.y), t.vc.y);
	auto z0 = min(min(t.va.z, t.vb.z), t.vc.z);
	auto z1 = max(max(t.va.z, t.vb.z), t.vc.z);
	bbox = aabb(x0, x1, y0, y1, z0, z1);
}
bool Tri::Hit(ray const& r, double tmin, double tmax, hit_rec& rec) const {
	// 说实话我不知道这里防止溢出的方式有没有用。
	vec3 w0 = t.va - r.o;
	vec3 w1 = t.vb - r.o;
	vec3 w2 = t.vc - r.o;
	mat3 m(w0, w1, w2); m = m.inv();
	vec3 v = m * r.v;
	if (v.x <= 0 || v.y <= 0 || v.z <= 0) { return false; }

	auto sum = v.x + v.y + v.z;
	if (!isfinite(sum)) { return false; }
	v /= sum;

	rec.m = mat;
	rec.u = v.x * t.ua + v.y * t.ub + v.z * t.uc;
	rec.p = v.x * t.va + v.y * t.vb + v.z * t.vc;
	rec.t = (rec.p - r.o).len();
	if (!insd(rec.t, tmin, tmax)) { return false; }
	// auto n = cross(v1 - v0, v2 - v0).unit();
	auto n = (v.x * t.na + v.y * t.nb + v.z * t.nc).unit();
	// 这样插值的话，散射时可能导致光线落到几何体内部。
	// 我们还是对这个问题视而不见算了。
	if (n.zero()) { n = t.na; }
	rec.deal_front(r, n); return true;
}
