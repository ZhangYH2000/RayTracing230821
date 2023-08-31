#pragma once
#include "vec3.h"

inline vec3 rnd_ball() {
	while (1) {
		vec3 v(frnd(-1, 1), frnd(-1, 1), frnd(-1, 1));
		if (v.lensqr() < 1) { return v; }
	}
}

inline vec3 reflect(vec3 v, vec3 n) { return v - 2 * dot(v, n) * n; }
inline vec3 refract(vec3 v, vec3 n, double eta_rel) {
	// 折射前的折射率是 eta_rel 的分子。
	auto c = dot(-v, n);
	vec3 out_perp = eta_rel * (v + c * n);
	vec3 out_para = -sqrt(abs(1 - out_perp.lensqr())) * n;
	return out_perp + out_para;
}

inline double schlick(double cosine, double eta_rel) {
	// Schlick 近似计算反射率，我不懂原理。
	auto r0 = (1 - eta_rel) / (1 + eta_rel);
	r0 = r0 * r0;
	return r0 + (1 - r0) * pow(1 - cosine, 5);
}

// 我觉得这里可以改一下，有一些遗留问题。
inline vec3 rnd_cap(double lsqr) {
	auto h = sqrt(abs(1 - lsqr));
	auto z = frnd(1 - h) + h;
	auto r = sqrt(1 - z * z);
	auto phi = frnd(2 * PI);
	auto x = cos(phi) * r;
	auto y = sin(phi) * r;
	return vec3(x, y, z);
}
