#pragma once
#include "pdf.h"
#include "helper.h"

struct CosPDF : PDF {
	vec3 n;
	CosPDF(vec3 n) : n(n) {}
	double Val(vec3 v) const override {
		double c = dot(v, n); 
		return max(0.0, c / PI);
	};
	vec3 Gen() const override { 
		auto v = n + rnd_ball();
		return v.near_zero(1e-3) ? n : v.unit();
	};
};
