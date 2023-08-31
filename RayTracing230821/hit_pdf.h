#pragma once
#include "pdf.h"
#include "hittable.h"

struct HitPDF : PDF {
	vec3 o;
	ptr<Hittable> ob;
	HitPDF(vec3 o, ptr<Hittable> ob) : o(o), ob(ob) {}
	double Val(vec3 v) const override { return ob->PDFVal(o, v); }
	vec3 Gen() const override { return ob->Rnd(o); }
};
