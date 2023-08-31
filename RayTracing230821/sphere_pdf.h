#pragma once
#include "pdf.h"
#include "helper.h"

struct SpherePDF : PDF {
	double Val(vec3 v) const override { return 1 / (4 * PI); };
	vec3 Gen() const override { return rnd_ball(); };
};
