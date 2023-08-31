#include "rotate.h"

bool Rotate::Hit(ray const& r, double tmin, double tmax, hit_rec& rec) const {
	ray r_tsl(inv_tsf * r.o, inv_tsf * r.v, r.in);
	if (!ob->Hit(r_tsl, tmin, tmax, rec)) { return false; }
	rec.p = tsf * rec.p; 
	rec.n = tsf * rec.n; return true;
}

double Rotate::PDFVal(vec3 o, vec3 v) const { return ob->PDFVal(inv_tsf * o, inv_tsf * v); }
vec3 Rotate::Rnd(vec3 o) const { return tsf * ob->Rnd(inv_tsf * o); }
