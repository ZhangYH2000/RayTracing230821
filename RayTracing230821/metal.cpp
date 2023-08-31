#include "metal.h"
#include "helper.h"
#include "hittable.h"

#include "cos_pdf.h"

bool Metal::Scatter(ray const& in, hit_rec const& rec, scatter_rec& srec) const {
	srec.skip_ray.o = rec.p;
	srec.skip_ray.in = in.in;
	srec.skip_ray.v = (reflect(in.v, rec.n) + fuzz * rnd_ball()).unit();
	srec.c = t->Val(rec.u, rec.p);
	srec.skip_pdf = true; 
	return dot(srec.skip_ray.v, rec.n) > 0;
}
