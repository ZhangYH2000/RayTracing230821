#include "refract.h"
#include "helper.h"

bool Refract::Scatter(ray const& in, hit_rec const& rec, scatter_rec& srec) const {
	srec.skip_pdf = true;
	srec.skip_ray.o = rec.p;
	srec.skip_ray.in = in.in;
	double eta_rel = rec.front ? 1 / ref_idx : ref_idx;
	// �Ҿ�������ļ�������Ż��ġ�
	double cth = min(1.0, dot(-in.v, rec.n));
	double sth = sqrt(1 - cth * cth);

	// �һ������⣬�ѵ���ǿ���ı�����ô͹͸���۽�Ч���ܲ���ʵ���أ�
	bool cant_fract = eta_rel * sth > 1;
	if (cant_fract || schlick(cth, eta_rel) > frnd(1)) {
		srec.c = 1;
		srec.skip_ray.v = reflect(in.v, rec.n);
	} else {
		srec.c = 1 / eta_rel;
		srec.skip_ray.in += rec.front ? 1 : -1;
		srec.skip_ray.in = max(srec.skip_ray.in, 0);
		srec.skip_ray.v = refract(in.v, rec.n, eta_rel);
	} return true;
}
