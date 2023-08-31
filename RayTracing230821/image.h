#pragma once
#include "tile.h"
#include "texture.h"

struct Image : Texture {
	ptr<tile> t;
	double b = 0;
	Image(ptr<tile> t, double b = 1) : t(t), b(b) {}
	col3 Val(vec2 u, vec3 p) const { 
		auto c = (col3)t->sample(u); 
		c.x *= c.x;
		c.y *= c.y;
		c.z *= c.z;
		// 记得改回来！
		return b * c;
	}
};
