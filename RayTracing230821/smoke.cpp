#include "smoke.h"
#include "helper.h"

bool Smoke::Scatter(ray const& in, hit_rec const& rec, scatter_rec &srec) const {
	srec.c = col;
	srec.in = in.in;
	srec.p = p;
	srec.pdf = pdf;
	srec.skip_pdf = false; return true;
}
