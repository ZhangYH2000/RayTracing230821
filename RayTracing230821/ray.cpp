#include "ray.h"
#include "helper.h"
#include "material.h"
#include "hittable.h"
#include "hit_pdf.h"
#include "mix_pdf.h"
#include "tile.h"
#include "cur.h"
#include "hit_item.h"

col3 ray_col(ray const& r, int dep, Cur &cur) {
	hit_rec rec; 
	if (dep <= 0) { return {}; }

	if (cur.world->Hit(r, 1e-5, DBL_MAX, rec)) {
		scatter_rec srec;
		col3 e = rec.m->Emit(rec.u, rec.p);
		if (!rec.m->Scatter(r, rec, srec)) { return e; } 
		
		if (srec.skip_pdf) {
			return e + srec.c * ray_col(srec.skip_ray, dep - 1, cur);
		}

		vector<pdf_item> tmp;
		for (auto& h : cur.hits) {
			if (h->pdf) { 
				tmp.push_back({ msh<HitPDF>(rec.p, h->hit), h->pdf });
			}
		}

		ptr<PDF> pdf;
		if (tmp.empty()) {
			pdf = srec.pdf;
		} else {
			pdf = msh<MixPDF>(srec.pdf, msh<MixPDF>(tmp), srec.p);
		}

		vec3 v = pdf->Gen();
		double pdf_val = pdf->Val(v);
		double spdf_val = srec.pdf->Val(v);
		if (spdf_val <= 0 || pdf_val <= 0) { return e; }
		col3 s = ray_col(ray(rec.p, v, srec.in), dep - 1, cur);
		return e + srec.c * s * spdf_val / pdf_val;
	}

	double a = 0.5 * (r.v.z + 1);
	return a * cur.c0_back + (1 - a) * cur.c1_back;
}
