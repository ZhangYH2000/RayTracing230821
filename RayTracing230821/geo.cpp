#include "geo.h"
#include "var.h"
#include "model.h"

#define gl (m.gl)
#define found(s) (g.find(s) != g.end())
#define getv(nm) if (found(L#nm)) { nm = g[L#nm]->num; }

geo::geo(model& m, FILE* f) {
	int sz = 0;
	frdv(cmd); frd(p); frd(tsf);
}
void geo::transform() {
	for (auto& f : faces)
	for (auto& t : f.tris) {
		t.va = p + tsf * t.va;
		t.vb = p + tsf * t.vb;
		t.vc = p + tsf * t.vc;
		t.na = tsf * t.na;
		t.nb = tsf * t.nb;
		t.nc = tsf * t.nc;
	}
}

void geo::refresh_sphere(model& m) {
	tri t; auto g = gl[L"geo"]->dic;
	faces.clear(); faces.resize(1);
	double r = 1;
	int nphi = 40, ntheta = 40;
	getv(r); getv(nphi); getv(ntheta);

	// 顶部和底部会有一些奇异的三角形，但是懒得管了。
	rep(i, 0, nphi) {
		double phi1 = PI * i / nphi;
		double phi0 = PI * (i + 1) / nphi;
		double sp0 = sin(phi0), cp0 = cos(phi0);
		double sp1 = sin(phi1), cp1 = cos(phi1);
		rep(j, 0, ntheta) {
			double theta0 = 2 * PI * j / ntheta;
			double theta1 = 2 * PI * (j + 1) / ntheta;
			double st0 = sin(theta0), ct0 = cos(theta0);
			double st1 = sin(theta1), ct1 = cos(theta1);
			vec3 p0 = r * vec3(ct0 * sp0, st0 * sp0, cp0);
			vec3 p1 = r * vec3(ct1 * sp0, st1 * sp0, cp0);
			vec3 p2 = r * vec3(ct1 * sp1, st1 * sp1, cp1);
			vec3 p3 = r * vec3(ct0 * sp1, st0 * sp1, cp1);

			t.va = p0; t.vb = p1; t.vc = p2;
			t.na = p0.unit(); 
			t.nb = p1.unit(); 
			t.nc = p2.unit();
			t.ua.x = (double)j / ntheta;
			t.ua.y = (double)(i + 1) / nphi;
			t.ub.x = (double)(j + 1) / ntheta;
			t.ub.y = (double)(i + 1) / nphi;
			t.uc.x = (double)(j + 1) / ntheta;
			t.uc.y = (double)i / nphi;
			faces[0].tris.push_back(t);
			t.va = p0; t.vb = p2; t.vc = p3;
			t.na = p0.unit();
			t.nb = p2.unit();
			t.nc = p3.unit();
			t.ua.x = (double)j / ntheta;
			t.ua.y = (double)(i + 1) / nphi;
			t.ub.x = (double)(j + 1) / ntheta;
			t.ub.y = (double)i / nphi;
			t.uc.x = (double)j / ntheta;
			t.uc.y = (double)i / nphi;
			faces[0].tris.push_back(t);
		}
	}
}
void geo::refresh_cylinder(model& m) {
	tri t; auto g = gl[L"geo"]->dic;
	faces.clear(); faces.resize(3);
	double r = 1, l = 1;
	int ntheta = 40;
	getv(r); getv(l); getv(ntheta);

	rep(i, 0, ntheta) {
		double theta0 = 2 * PI * i / ntheta;
		double theta1 = 2 * PI * (i + 1) / ntheta;
		double st0 = sin(theta0), ct0 = cos(theta0);
		double st1 = sin(theta1), ct1 = cos(theta1);
		vec3 p0 = vec3(-l / 2, -ct0 * r, st0 * r);
		vec3 p1 = vec3(+l / 2, -ct0 * r, st0 * r);
		vec3 p2 = vec3(+l / 2, -ct1 * r, st1 * r);
		vec3 p3 = vec3(-l / 2, -ct1 * r, st1 * r);

		t.va = p0; t.vb = p1; t.vc = p2;
		t.na = t.nb = vec3(0, -ct0, st0);
		t.nc = vec3(0, -ct1, st1);
		t.ua.x = 0;
		t.ua.y = 1 - (double)i / ntheta;
		t.ub.x = 1;
		t.ub.y = 1 - (double)i / ntheta;
		t.uc.x = 1;
		t.uc.y = 1 - (double)(i + 1) / ntheta;
		faces[0].tris.push_back(t);
		t.va = p0; t.vb = p2; t.vc = p3;
		t.na = vec3(0, -ct0, st0);
		t.nb = t.nc = vec3(0, -ct1, st1);
		t.ua.x = 0;
		t.ua.y = 1 - (double)i / ntheta;
		t.ub.x = 1;
		t.ub.y = 1 - (double)(i + 1) / ntheta;
		t.uc.x = 0;
		t.uc.y = 1 - (double)(i + 1) / ntheta;
		faces[0].tris.push_back(t);
	}

	rep(i, 0, ntheta) {
		double theta0 = 2 * PI * i / ntheta;
		double theta1 = 2 * PI * (i + 1) / ntheta;
		double st0 = sin(theta0), ct0 = cos(theta0);
		double st1 = sin(theta1), ct1 = cos(theta1);
		vec3 p0 = vec3(-l / 2, 0, 0);
		vec3 p1 = vec3(-l / 2, -ct0 * r, st0 * r);
		vec3 p2 = vec3(-l / 2, -ct1 * r, st1 * r);

		t.va = p0; t.vb = p1; t.vc = p2;
		t.na = t.nb = t.nc = vec3(-1, 0, 0);
		t.ua = vec2(0.5);
		t.ub = vec2(0.5) + 0.5 * vec2(ct0, -st0);
		t.uc = vec2(0.5) + 0.5 * vec2(ct1, -st1);
		faces[1].tris.push_back(t);
	}

	rep(i, 0, ntheta) {
		double theta0 = 2 * PI * i / ntheta;
		double theta1 = 2 * PI * (i + 1) / ntheta;
		double st0 = sin(theta0), ct0 = cos(theta0);
		double st1 = sin(theta1), ct1 = cos(theta1);
		vec3 p0 = vec3(l / 2, 0, 0);
		vec3 p1 = vec3(l / 2, -ct1 * r, st1 * r);
		vec3 p2 = vec3(l / 2, -ct0 * r, st0 * r);

		t.va = p0; t.vb = p1; t.vc = p2;
		t.na = t.nb = t.nc = vec3(1, 0, 0);
		t.ua = vec2(0.5);
		t.ub = vec2(0.5) + 0.5 * vec2(-ct1, -st1);
		t.uc = vec2(0.5) + 0.5 * vec2(-ct0, -st0);
		faces[2].tris.push_back(t);
	}
}
void geo::refresh_box(model& m) {
	vec3 p0, p1, p2, p3; tri t;
	auto g = gl[L"geo"]->dic;
	faces.clear(); faces.resize(6);
	double a = 1, b = 1, c = 1;
	getv(a); getv(b); getv(c);

	p0 = vec3(-a, -b, -c) / 2;
	p1 = vec3(+a, -b, -c) / 2;
	p2 = vec3(+a, -b, +c) / 2;
	p3 = vec3(-a, -b, +c) / 2;
	t.va = p0; t.vb = p1; t.vc = p2;
	t.na = t.nb = t.nc = vec3(0, -1, 0);
	t.ua = vec2(0, 1);
	t.ub = vec2(1, 1);
	t.uc = vec2(1, 0);
	faces[0].tris.push_back(t);
	t.va = p0; t.vb = p2; t.vc = p3;
	t.ua = vec2(0, 1);
	t.ub = vec2(1, 0);
	t.uc = vec2(0, 0);
	faces[0].tris.push_back(t);

	p0 = vec3(+a, +b, -c) / 2;
	p1 = vec3(-a, +b, -c) / 2;
	p2 = vec3(-a, +b, +c) / 2;
	p3 = vec3(+a, +b, +c) / 2;
	t.va = p0; t.vb = p1; t.vc = p2;
	t.na = t.nb = t.nc = vec3(0, 1, 0);
	t.ua = vec2(0, 1);
	t.ub = vec2(1, 1);
	t.uc = vec2(1, 0);
	faces[1].tris.push_back(t);
	t.va = p0; t.vb = p2; t.vc = p3;
	t.ua = vec2(0, 1);
	t.ub = vec2(1, 0);
	t.uc = vec2(0, 0);
	faces[1].tris.push_back(t);

	p0 = vec3(-a, +b, -c) / 2;
	p1 = vec3(-a, -b, -c) / 2;
	p2 = vec3(-a, -b, +c) / 2;
	p3 = vec3(-a, +b, +c) / 2;
	t.va = p0; t.vb = p1; t.vc = p2;
	t.na = t.nb = t.nc = vec3(-1, 0, 0);
	t.ua = vec2(0, 1);
	t.ub = vec2(1, 1);
	t.uc = vec2(1, 0);
	faces[2].tris.push_back(t);
	t.va = p0; t.vb = p2; t.vc = p3;
	t.ua = vec2(0, 1);
	t.ub = vec2(1, 0);
	t.uc = vec2(0, 0);
	faces[2].tris.push_back(t);

	p0 = vec3(+a, -b, -c) / 2;
	p1 = vec3(+a, +b, -c) / 2;
	p2 = vec3(+a, +b, +c) / 2;
	p3 = vec3(+a, -b, +c) / 2;
	t.va = p0; t.vb = p1; t.vc = p2;
	t.na = t.nb = t.nc = vec3(1, 0, 0);
	t.ua = vec2(0, 1);
	t.ub = vec2(1, 1);
	t.uc = vec2(1, 0);
	faces[3].tris.push_back(t);
	t.va = p0; t.vb = p2; t.vc = p3;
	t.ua = vec2(0, 1);
	t.ub = vec2(1, 0);
	t.uc = vec2(0, 0);
	faces[3].tris.push_back(t);

	p0 = vec3(-a, -b, +c) / 2;
	p1 = vec3(+a, -b, +c) / 2;
	p2 = vec3(+a, +b, +c) / 2;
	p3 = vec3(-a, +b, +c) / 2;
	t.va = p0; t.vb = p1; t.vc = p2;
	t.na = t.nb = t.nc = vec3(0, 0, 1);
	t.ua = vec2(0, 1);
	t.ub = vec2(1, 1);
	t.uc = vec2(1, 0);
	faces[4].tris.push_back(t);
	t.va = p0; t.vb = p2; t.vc = p3;
	t.ua = vec2(0, 1);
	t.ub = vec2(1, 0);
	t.uc = vec2(0, 0);
	faces[4].tris.push_back(t);

	p0 = vec3(-a, +b, -c) / 2;
	p1 = vec3(+a, +b, -c) / 2;
	p2 = vec3(+a, -b, -c) / 2;
	p3 = vec3(-a, -b, -c) / 2;
	t.va = p0; t.vb = p1; t.vc = p2;
	t.na = t.nb = t.nc = vec3(0, 0, -1);
	t.ua = vec2(0, 1);
	t.ub = vec2(1, 1);
	t.uc = vec2(1, 0);
	faces[5].tris.push_back(t);
	t.va = p0; t.vb = p2; t.vc = p3;
	t.ua = vec2(0, 1);
	t.ub = vec2(1, 0);
	t.uc = vec2(0, 0);
	faces[5].tris.push_back(t);
}
void geo::refresh_cone(model& m) {
	tri t; auto g = gl[L"geo"]->dic;
	faces.clear(); faces.resize(2);
	double r = 1, l = 1;
	int ntheta = 40;
	getv(r); getv(l); getv(ntheta);

	rep(i, 0, ntheta) {
		double theta0 = 2 * PI * i / ntheta;
		double theta1 = 2 * PI * (i + 1) / ntheta;
		double st0 = sin(theta0), ct0 = cos(theta0);
		double st1 = sin(theta1), ct1 = cos(theta1);
		vec3 p0 = vec3(-l / 2, -ct1 * r, st1 * r);
		vec3 p1 = vec3(-l / 2, -ct0 * r, st0 * r);
		vec3 p2 = vec3(l / 2, 0, 0);

		t.va = p0; t.vb = p1; t.vc = p2;
		t.na = vec3(r, -ct1 * l, st1 * l).unit();
		t.nb = vec3(r, -ct0 * l, st0 * l).unit();
		double theta = (theta0 + theta1) / 2;
		t.nc = vec3(r, -cos(theta) * l, sin(theta) * l).unit();
		
		t.ua = vec2(0.5) + 0.5 * vec2(ct0, -st0);
		t.ub = vec2(0.5) + 0.5 * vec2(ct1, -st1);
		t.uc = vec2(0.5);
		faces[0].tris.push_back(t);
	}

	rep(i, 0, ntheta) {
		double theta0 = 2 * PI * i / ntheta;
		double theta1 = 2 * PI * (i + 1) / ntheta;
		double st0 = sin(theta0), ct0 = cos(theta0);
		double st1 = sin(theta1), ct1 = cos(theta1);
		vec3 p0 = vec3(-l / 2, 0, 0);
		vec3 p1 = vec3(-l / 2, -ct0 * r, st0 * r);
		vec3 p2 = vec3(-l / 2, -ct1 * r, st1 * r);

		t.va = p0; t.vb = p1; t.vc = p2;
		t.na = t.nb = t.nc = vec3(-1, 0, 0);
		t.ua = vec2(0.5);
		t.ub = vec2(0.5) + 0.5 * vec2(ct0, -st0);
		t.uc = vec2(0.5) + 0.5 * vec2(ct1, -st1);
		faces[1].tris.push_back(t);
	}
}
void geo::refresh_pyramid(model& m) {
	vec3 p0, p1, p2, p3;
	tri t; auto g = gl[L"geo"]->dic;
	faces.clear(); faces.resize(5);
	double a = 1, b = 1, c = 1;
	getv(a); getv(b); getv(c);

	p0 = vec3(-a, +b, -c) / 2;
	p1 = vec3(+a, +b, -c) / 2;
	p2 = vec3(+a, -b, -c) / 2;
	p3 = vec3(-a, -b, -c) / 2;
	t.va = p0; t.vb = p1; t.vc = p2;
	t.na = t.nb = t.nc =
		cross(t.vb - t.va, t.vc - t.va).unit();
	t.ua = vec2(0, 1);
	t.ub = vec2(1, 1);
	t.uc = vec2(1, 0);
	faces[0].tris.push_back(t);
	t.va = p0; t.vb = p2; t.vc = p3;
	t.na = t.nb = t.nc =
		cross(t.vb - t.va, t.vc - t.va).unit();
	t.ua = vec2(0, 1);
	t.ub = vec2(1, 0);
	t.uc = vec2(0, 0);
	faces[0].tris.push_back(t);

	p2 = vec3(0, 0, c) / 2;
	p0 = vec3(-a, -b, -c) / 2;
	p1 = vec3(+a, -b, -c) / 2;
	t.va = p0; t.vb = p1; t.vc = p2;
	t.na = t.nb = t.nc =
		cross(t.vb - t.va, t.vc - t.va).unit();
	t.ua = vec2(0, 1);
	t.ub = vec2(1, 1);
	t.uc = vec2(0.5, 0);
	faces[1].tris.push_back(t);
	p0 = vec3(+a, +b, -c) / 2;
	p1 = vec3(-a, +b, -c) / 2;
	t.va = p0; t.vb = p1; t.vc = p2;
	t.na = t.nb = t.nc =
		cross(t.vb - t.va, t.vc - t.va).unit();
	t.ua = vec2(0, 1);
	t.ub = vec2(1, 1);
	t.uc = vec2(0.5, 0);
	faces[2].tris.push_back(t);
	p0 = vec3(-a, +b, -c) / 2;
	p1 = vec3(-a, -b, -c) / 2;
	t.va = p0; t.vb = p1; t.vc = p2;
	t.na = t.nb = t.nc =
		cross(t.vb - t.va, t.vc - t.va).unit();
	t.ua = vec2(0, 1);
	t.ub = vec2(1, 1);
	t.uc = vec2(0.5, 0);
	faces[3].tris.push_back(t);
	p0 = vec3(+a, -b, -c) / 2;
	p1 = vec3(+a, +b, -c) / 2;
	t.va = p0; t.vb = p1; t.vc = p2;
	t.na = t.nb = t.nc =
		cross(t.vb - t.va, t.vc - t.va).unit();
	t.ua = vec2(0, 1);
	t.ub = vec2(1, 1);
	t.uc = vec2(0.5, 0);
	faces[4].tris.push_back(t);
}
void geo::refresh_user(model& m) {
	auto fs = gl[L"geo"]->vec;
	faces.clear(); faces.resize(fs.size());

	rep(i, 0, faces.size()) {
		faces[i].tris = m.get_face(fs[i]->str);
	}
}
void geo::refresh(model& m) {
	gl[L"geo"] = msh<Var>();
	gl[L"faces"] = msh<Var>();
	Execute(gl, Compile(cmd));
	auto g = gl[L"geo"]->dic;

	wstring type;
	if (found(L"type")) { type = g[L"type"]->str; }
	if (type == L"sphere") { refresh_sphere(m); }
	else if (type == L"cylinder") { refresh_cylinder(m); }
	else if (type == L"box") { refresh_box(m); }
	else if (type == L"cone") { refresh_cone(m); }
	else if (type == L"pyramid") { refresh_pyramid(m); }
	else { refresh_user(m); }

	if (found(L"p")) { p = tv3(*g[L"p"]); }
	if (found(L"transform")) {
		auto& v = g[L"transform"]->vec;
		if (v.size() >= 9) {
			tsf.a00 = v[0]->num;
			tsf.a01 = v[1]->num;
			tsf.a02 = v[2]->num;
			tsf.a10 = v[3]->num;
			tsf.a11 = v[4]->num;
			tsf.a12 = v[5]->num;
			tsf.a20 = v[6]->num;
			tsf.a21 = v[7]->num;
			tsf.a22 = v[8]->num;
		}
	} transform();

	auto fs = gl[L"faces"]->vec;
	rep(i, 0, min(faces.size(), fs.size())) {
		auto& v = *fs[i];
		if (v.typ == L"vec") { faces[i].c = tv3(v.vec); }
		if (v.typ == L"str") {
			faces[i].t = tile(L"images/" + v.str + L".tile");
			if (faces[i].t.n()) {
				faces[i].use_tile = true;
			}
		}
	}
}
