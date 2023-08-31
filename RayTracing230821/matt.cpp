#include "matt.h"
#include "helper.h"
#include "cos_pdf.h"

bool Matt::Scatter(ray const& in, hit_rec const& rec, scatter_rec& srec) const {
	srec.c = t->Val(rec.u, rec.p);
	srec.in = in.in;
	srec.p = p;
	srec.pdf = msh<CosPDF>(rec.n);
	srec.skip_pdf = false; return true;
}
