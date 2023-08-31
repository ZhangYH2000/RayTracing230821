#include "translate.h"

bool Translate::Hit(ray const& r, double tmin, double tmax, hit_rec& rec) const {
	ray r_tsl(r.o - offset, r.v, r.in);
	if (!ob->Hit(r_tsl, tmin, tmax, rec)) { return false; }
	rec.p += offset; return true;
}

double Translate::PDFVal(vec3 o, vec3 v) const { return ob->PDFVal(o - offset, v); }
vec3 Translate::Rnd(vec3 o) const { return ob->Rnd(o - offset); }
