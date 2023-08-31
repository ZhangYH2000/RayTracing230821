#pragma once
#include "texture.h"

struct Checker : Texture {
	double inv_scl = 0;
	col3 even, odd;

	Checker(double scl, col3 e, col3 o) : inv_scl(1 / scl), even(e), odd(o) {}
	col3 Val(vec2 u, vec3 p) const override;
};
