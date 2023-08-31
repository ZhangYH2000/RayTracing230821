#pragma once
#include "vec3.h"

struct Cur;
struct Hittable;
struct hit_item {
	vec3 p;
	mat3 tsf = mat3::id();
	wstring cmd, tmp_cmd;
	ptr<Hittable> hit, pre_hit;
	double pdf = 0;
	bool del = false;
	bool show = true;

	hit_item() = default;
	hit_item(Cur &cur, FILE* f);
	void save(FILE* f) const;
	void total_refresh(Cur &cur);
	void refresh(Cur& cur);
};
