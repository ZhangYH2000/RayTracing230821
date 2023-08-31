#include "sphere.h"
#include "helper.h"
#include "var.h"
#include "texture.h"
#include "matt.h"
#include "metal.h"
#include "glass.h"
#include "model.h"
#include "smoke.h"
#include "col.h"
#include "image.h"
#include "light.h"
#include "checker.h"
#include "mix.h"

#define found(s) (dic.find(s) != dic.end())
#define getv(nm) if (found(L#nm)) { nm = dic.at(L#nm)->num; }

Sphere::Sphere(Var const &v) {
	auto dic = v.dic;
	getv(rad); 
	if (found(L"center")) { ct = tv3(*dic[L"center"]); }

	bool checker = false; getv(checker);
	double p_scatter = 1; getv(p_scatter);
	double fuzz = 0; getv(fuzz);
	bool smoke = false; getv(smoke);
	bool glass = false; getv(glass);
	bool gel = false; getv(gel);
	bool light = false; getv(light);
	double weight = 0.7; getv(weight);
	col3 c = 0.7;
	if (found(L"col")) { c = tv3(*dic.at(L"col")); }
	ptr<Texture> tex = msh<Col>(c);
	if (checker) {
		double scl_checker = 1; getv(scl_checker);
		col3 c0 = 0.2, c1 = 0.8;
		if (found(L"c0_checker")) { c0 = tv3(*dic.at(L"c0_checker")); }
		if (found(L"c1_checker")) { c1 = tv3(*dic.at(L"c1_checker")); }
		tex = msh<Checker>(scl_checker, c0, c1);
	}

	auto matt = msh<Matt>(tex); matt->p = weight;
	auto metal = msh<Metal>(tex, fuzz);
	mat = msh<Mix>(matt, metal, p_scatter);
	if (light) { mat = msh<Light>(tex); }
	if (smoke || glass || gel) { mat = NULL; }
}
Sphere::Sphere(vec3 ct, double r, ptr<Material> m) : ct(ct), rad(r), mat(m) {
	bbox = aabb(ct.x - r, ct.x + r, ct.y - r, ct.y + r, ct.z - r, ct.z + r);
}
bool Sphere::Hit(ray const& r, double tmin, double tmax, hit_rec& rec) const {
	vec3 co = r.o - ct;
	auto a = r.v.lensqr();
	auto hb = dot(co, r.v);
	auto c = co.lensqr() - rad * rad;

	auto disc = hb * hb - a * c;
	if (disc < 0) { return false; }
	auto sqrd = sqrt(disc);

	auto root = (-hb - sqrd) / a;
	if (!insd(root, tmin, tmax)) {
		root = (-hb + sqrd) / a;
		if (!insd(root, tmin, tmax)) { return false; }
	}

	rec.m = mat;
	rec.t = root;
	rec.p = r.at(rec.t);
	// 不知道要不要考虑浮点数溢出的问题。
	vec3 v = (rec.p - ct) / rad;
	rec.u.y = acos(v.z) / PI;
	rec.u.x = atan2(v.y, v.x) / (2 * PI) + 0.5;
	rec.deal_front(r, v); return true;
}

double Sphere::PDFVal(vec3 o, vec3 v) const {
	auto lsqr = rad * rad / (ct - o).lensqr();
	if (lsqr >= 1) { return 1 / (4 * PI); }
	auto h = sqrt(abs(1 - lsqr));
	auto area = 2 * PI * (1 - h);
	// 对浮点数异常的分析我还是不太清楚。
	// 这个溢出了其实没关系的对吧。
	return 1 / area;
}
vec3 Sphere::Rnd(vec3 o) const {
	// 不知道这个效率怎么样。
	auto dsqr = (ct - o).lensqr();
	auto lsqr = rad * rad / dsqr;
	if (lsqr >= 1) { return rnd_ball(); }

	auto v = rnd_cap(lsqr);
	auto n = (ct - o).unit();
	auto z = vec3(0, 0, 1);
	if (n == z) { return v; }
	auto zn = (n - z).unit();
	return reflect(v, zn);
}
