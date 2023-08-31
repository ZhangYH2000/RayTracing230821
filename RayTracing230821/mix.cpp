#include "mix.h"

Mix::Mix(ptr<Material> m0, ptr<Material> m1, double p0) {
	mat_item item0{ m0, p0 };
	mat_item item1{ m1, 1 - p0 };
	ms = { item0, item1 };
}

bool Mix::Scatter(ray const& in, hit_rec const& rec, scatter_rec& srec) const {
	double p = frnd(1);
	for (auto& m : ms) {
		p -= m.p;
		if (p <= 0) { return m.mat->Scatter(in, rec, srec); }
	}
	return ms.back().mat->Scatter(in, rec, srec);
}
col3 Mix::Emit(vec2 u, vec3 p) const {
	double prob = frnd(1);
	for (auto& m : ms) {
		prob -= m.p;
		if (prob <= 0) { return m.mat->Emit(u, p); }
	}
	return ms.back().mat->Emit(u, p);
}
