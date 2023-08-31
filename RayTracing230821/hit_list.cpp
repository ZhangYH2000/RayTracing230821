#include "hit_list.h"

void HitList::Add(ptr<Hittable> ob) { obs.push_back(ob); }
bool HitList::Hit(ray const& r, double tmin, double tmax, hit_rec& rec) const {
	hit_rec tmp;
	bool hit = false;
	
	for (auto ob : obs) 
	if (ob->Hit(r, tmin, tmax, tmp)) {
		hit = true; tmax = tmp.t; 
		rec = tmp;
	}  return hit;
}
