#pragma once
#include "texture.h"
#include "material.h"

struct Matt : Material {
	ptr<Texture> t;
	double p = 0;
	Matt(ptr<Texture> t) : t(t) {}
	bool Scatter(ray const& in, hit_rec const& rec, scatter_rec& srec) const override;
};
