#pragma once
#include "pdf.h"
#include "hittable.h"

struct scatter_rec {
	col3 c;
	int in = 0;
	double p = 0;
	// ��� skip_pdf����ô in ���Բ��ܡ�
	ptr<PDF> pdf;
	bool skip_pdf = false;
	ray skip_ray;
	
};

struct Material {
	virtual ~Material() {}
	virtual bool Scatter
	(ray const& in, hit_rec const& rec, scatter_rec& srec) const { return false; };
	virtual col3 Emit(vec2 u, vec3 p) const { return {}; }
};
