#pragma once
#include "vec3.h"

struct PDF {
	virtual ~PDF() {}
	virtual double Val(vec3 v) const = 0;
	virtual vec3 Gen() const = 0;
};
