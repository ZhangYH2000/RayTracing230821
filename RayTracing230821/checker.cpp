#include "checker.h"

col3 Checker::Val(vec2 u, vec3 p) const {
	int x = floor(inv_scl * p.x);
	int y = floor(inv_scl * p.y);
	int z = floor(inv_scl * p.z);
	return (x + y + z) % 2 == 0 ? even : odd;
}
