#pragma once
#include "tile.h"

struct tri {
	vec3 va, vb, vc;
	vec2 ua, ub, uc;
	vec3 na, nb, nc;
};
struct face {
	col3 c;
	tile t;
	bool use_tile = false;
	vector<tri> tris;
};

struct model;
struct geo {
	vec3 p;
	mat3 tsf;
	vector<face> faces;
	wstring cmd;

	geo(model& m, FILE* f);
	void transform();

	void refresh_sphere(model& m);
	void refresh_cylinder(model& m);
	void refresh_box(model& m);
	void refresh_cone(model& m);
	void refresh_pyramid(model& m);
	void refresh_user(model& m);
	void refresh(model& m);
};
