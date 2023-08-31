#pragma once
#include "texture.h"

struct Col : Texture {
	col3 col;
	Col(col3 c) : col(c) {}
	col3 Val(vec2 u, vec3 p) const override { return col; }
};
