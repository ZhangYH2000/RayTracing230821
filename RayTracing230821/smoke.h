#pragma once
#include "sphere_pdf.h"
#include "material.h"

struct Smoke : Material {
	col3 col;
	double p = 0;
	ptr<SpherePDF> pdf;
	Smoke(col3 c) : col(c), pdf(msh<SpherePDF>()) {}
	bool Scatter(ray const& in, hit_rec const& rec, scatter_rec& srec) const override;
};
