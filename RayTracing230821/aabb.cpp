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

	// ʵ����������ȡ�ϸ񲻵Ⱥ�����Ҫ�ģ������ε������п��ܺ��Ϊ 0��
	// �и�����������ͳ��� 0 ����ô����ġ�
	// ������ 0 / 0 �����⣬���������ﲻ��Ҫ��
	double t0 = 0, t1 = 0;
	TMP(x); TMP(y); TMP(z); return true;
#undef TMP
}
