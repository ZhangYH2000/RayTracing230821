#include "inst.h"
#include "osc.h"
#include "filter.h"
#include "rhythm.h"
#include "lfo.h"
#include "env.h"
#include "var.h"

#define gl (ry.gl)

Inst::Inst(rhythm& ry) {
#define find(nm) gl.find(L#nm) != gl.end()

#define get(nm) if(find(nm)) { nm = rthm_num(ry, *gl.at(L#nm)); }
	get(vol);
#undef get

#define get(nm) if (find(nm)) { nm = gl.at(L#nm)->num; }
	get(fixed_tone);
#undef get

	if (find(lfo)) {
		auto& vs = gl[L"lfo"]->vec;
		for (auto v : vs) { lfos.push_back(msh<LFO>(ry, *v)); }
	}
	if (find(env)) {
		auto& vs = gl[L"env"]->vec;
		for (auto v : vs) { envs.push_back(msh<Env>(ry, *v)); }
	}
	if (find(osc)) {
		auto& vs = gl[L"osc"]->vec;
		for (auto v : vs) { oscs.push_back(msh<Osc>(ry, *v)); }
	}

	// 懒得再去考虑主音量了。
	for (auto osc : oscs) if (osc->vol.type == TYP_ENV)
	if (osc->vol.id < envs.size()) {
		auto& r = envs[osc->vol.id]->release;
		t_end_rel = max(t_end_rel, r.type == TYP_NUM ? r.num : r.a + r.b);
		// 这里隐含了 a 必须为正数。
	}
#undef find
}

Inst::Inst(Inst const& inst) {
#define copy(nm) nm = inst.nm;
	copy(tone); copy(t_end_rel); copy(t_release);
	copy(vol); copy(fixed_tone); copy(t); copy(end);
#undef copy
	for (auto a : inst.lfos) { lfos.push_back(msh<LFO>(*a)); }
	for (auto a : inst.envs) { envs.push_back(msh<Env>(*a)); }
	for (auto a : inst.oscs) { oscs.push_back(msh<Osc>(*a)); }
}
void Inst::Set(int tn, double tr) { tone = tn; t_release = tr; }

#define sp (ry.sp)

double Inst::Sample(rhythm& ry) {
	if (end) { return 0; } t += 1.0 / sp;
	if (t > t_release + t_end_rel) { end = true; }
	double out = 0;
	for (auto o : oscs) { out += vol.val(ry, * this) * o->Sample(ry, *this); }
	return out;
}
