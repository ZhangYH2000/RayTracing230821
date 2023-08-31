#pragma once
#include "texture.h"
#include "material.h"

struct Light : Material {
	ptr<Texture> t;
	Light(ptr<Texture> t) : t(t) {}
	col3 Emit(vec2 u, vec3 p) const { return t->Val(u, p); }
};
