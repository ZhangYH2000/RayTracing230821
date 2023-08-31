#include "medium.h"
#include "helper.h"

bool Medium::Hit(ray const& r, double tmin, double tmax, hit_rec& rec) const {
	if (!shape->Hit(r, tmin, DBL_MAX, rec)) { return false; }
	
	bool bd = r.in == 0 || (r.in == 1 && !rec.front);
	rec.m = bd ? border : pass;
	if (!r.in) { return rec.t < tmax; }
	// 这里的浮点数溢出我还不知道怎么处理。
	double t_hit = max(tmin, neg_inv_density * log(frnd(1)));
	if (t_hit > rec.t) { return rec.t < tmax; }
	rec.t = t_hit; 
	rec.m = phase;
	rec.p = r.at(t_hit); return t_hit < tmax;
}
