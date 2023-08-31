#include "osc.h"
#include "rhythm.h"
#include "var.h"
#include "env.h"
#include "lfo.h"
#include "inst.h"
#include "filter.h"

Mouth::Mouth(double ph, double lv) : ph(ph), lv(lv) {}
double Mouth::Sample(double dph, int type) {
	ph = modf(ph + dph, 1);
	switch (type) {
	case WV_SQR: return ph > 0.5 ? 1 : -1;
	case WV_SAW: return 2 * ph - 1;
	case WV_TRI: return abs(2 - 4 * ph) - 1;
	case WV_NOI: return frnd(-1, 1);
	case WV_SIN: return sin(2 * PI * ph);
	} return 0;
}
double Mouth::Sample(double dph, vector<double> const& ys) {
	ph = modf(ph + dph, 1); return get_val(ph, ys);
}

FM::FM(rhythm& ry, Var const& v) {
#define find(nm) v.dic.find(L#nm) != v.dic.end()
#define get(nm) if (find(nm)) { nm = v.dic.at(L#nm)->num; }
	get(type); get(id);
#undef get
#define get(nm) if(find(nm)) { nm = rthm_num(ry, *v.dic.at(L#nm)); }
	get(amplitude);
#undef get
#undef find
}

Osc::Osc(rhythm& ry, Var const& v) {
	vol = 1;
#define find(nm) v.dic.find(L#nm) != v.dic.end()
	if (find(type)) {
		auto& tmp = *v.dic.at(L"type");
		if (tmp.typ == L"num") { type = tmp.num; }
		else { nm_fun = tmp.str; user_shape = true; }
	}
	vector<double> phs;
	if (find(phase)) for (auto ph : v.dic.at(L"phase")->vec) {
		phs.push_back(ph->num);
	}

#define get(nm) if(find(nm)) { nm = rthm_num(ry, *v.dic.at(L#nm)); }
	get(vol); get(transpose); get(detune); get(detune_power);
#undef get

	int unison = 0;
#define get(nm) if (find(nm)) { nm = v.dic.at(L#nm)->num; }
	get(unison); get(use);
#undef get
	unison = max(1, unison);
	rep(i, phs.size(), unison) { phs.push_back(frnd(0, 2 * PI)); }

	int k = 0;
	if (unison % 2 == 0) {
		rep(i, 0, unison / 2) {
			ms.push_back(Mouth(phs[k++], +0.5 + i));
			ms.push_back(Mouth(phs[k++], -0.5 - i));
		}
	} else {
		ms.push_back(Mouth(phs[k++], 0));
		rep(i, 1, unison / 2 + 1) {
			ms.push_back(Mouth(phs[k++], +i));
			ms.push_back(Mouth(phs[k++], -i));
		}
	}

	if (find(fm)) {
		auto& vs = v.dic.at(L"fm")->vec;
		for (auto v : vs) { fms.push_back(FM(ry, *v)); }
	}
	if (find(filter)) {
		auto& vs = v.dic.at(L"filter")->vec;
		for (auto v : vs) { fs.push_back(msh<Filter>(ry, *v)); }
	}

#undef find
}

Osc::Osc(Osc const& osc) {
#define copy(nm) nm = osc.nm;
	copy(type); copy(use); copy(user_shape); copy(nm_fun);
	copy(vol); copy(transpose); copy(detune); copy(detune_power);
	copy(val); copy(fms); copy(ms);
#undef copy
	for (auto f : osc.fs) { fs.push_back(msh<Filter>(*f)); }
}

#define sp (ry.sp)
#define oscs (inst.oscs)
#define envs (inst.envs)
#define lfos (inst.lfos)

double Osc::Sample(rhythm &ry, Inst& inst) {
	vector<double>* ys = NULL;
	val = 0; double fm = 0, sum = 0;
	for (auto& f : fms) {
		int id = f.id;
		double a = f.amplitude.val(ry, inst);
		switch (f.type) {
		case FM_OSC:
			if (insd<int>(id, 0, oscs.size())) {
				fm += oscs[id]->val * a;
			} break;
		case FM_ENV:
			if (insd<int>(id, 0, envs.size())) {
				fm += envs[id]->val(ry, inst) * a;
			} break;
		case FM_LFO:
			if (insd<int>(id, 0, lfos.size())) {
				fm += lfos[id]->val(ry, inst) * a;
			} break;
		}
	}

	if (user_shape) {
		if (ry.funs.find(nm_fun) == ry.funs.end()) { return 0; }
		ys = &*ry.funs.at(nm_fun);
	}
	double tsp = transpose.val(ry, inst);
	double dtn = detune.val(ry, inst);
	double dtp = detune_power.val(ry, inst);

	for (auto& m : ms) {
		double tn = inst.fixed_tone ? 0 : inst.tone;
		tn += dtn * m.lv + tsp;
		if (!inst.fixed_tone) { tn += ry.transpose; }
		double dph = 261.6 * exp2(tn / 12) * (1 + fm) / sp;
		double vol = pow(1 + dtp, abs(m.lv)); sum += vol;

		if (user_shape) { val += vol * m.Sample(dph, *ys); }
		else { val += vol * m.Sample(dph, type); }
	} 

	val *= vol.val(ry, inst) / sum;
	for (auto f : fs) { val = f->loop(ry, inst, val); }
	return use ? val : 0;
}
