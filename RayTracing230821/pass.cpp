#include "pass.h"

bool Pass::Scatter(ray const& in, hit_rec const& rec, scatter_rec& srec) const {
	srec.c = 1;
	srec.skip_pdf = true;
	srec.skip_ray.o = rec.p;
	srec.skip_ray.v = in.v;
	srec.skip_ray.in = in.in;
	srec.skip_ray.in += rec.front ? 1 : -1;
	srec.skip_ray.in = max(srec.skip_ray.in, 0); return true;
}
