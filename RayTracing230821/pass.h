#pragma once
#include "material.h"

struct Pass : Material {
	bool Scatter(ray const& in, hit_rec const& rec, scatter_rec& srec) const override;
};
