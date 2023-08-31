#pragma once
#include "vec3.h"

struct Texture {
	virtual ~Texture() {}
	virtual col3 Val(vec2 v, vec3 p) const = 0;
};
