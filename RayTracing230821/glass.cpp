#include "glass.h"
#include "helper.h"

bool Glass::Scatter(ray const& in, hit_rec const& rec, scatter_rec& srec) const {
	srec.c = col;
	srec.skip_pdf = true;
	srec.skip_ray.in = in.in;
	srec.skip_ray.v = (in.v + fuzz * rnd_ball()).unit();
	srec.skip_ray.o = rec.p;
	return true;
}
