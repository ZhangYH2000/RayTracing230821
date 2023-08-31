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
	// ����ǰ���������� eta_rel �ķ��ӡ�
	auto c = dot(-v, n);
	vec3 out_perp = eta_rel * (v + c * n);
	vec3 out_para = -sqrt(abs(1 - out_perp.lensqr())) * n;
	return out_perp + out_para;
}

inline double schlick(double cosine, double eta_rel) {
	// Schlick ���Ƽ��㷴���ʣ��Ҳ���ԭ��
	auto r0 = (1 - eta_rel) / (1 + eta_rel);
	r0 = r0 * r0;
	return r0 + (1 - r0) * pow(1 - cosine, 5);
}

// �Ҿ���������Ը�һ�£���һЩ�������⡣
inline vec3 rnd_cap(double lsqr) {
	auto h = sqrt(abs(1 - lsqr));
	auto z = frnd(1 - h) + h;
	auto r = sqrt(1 - z * z);
	auto phi = frnd(2 * PI);
	auto x = cos(phi) * r;
	auto y = sin(phi) * r;
	return vec3(x, y, z);
}
