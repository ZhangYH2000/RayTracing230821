#include "lfo.h"
#include "rhythm.h"
#include "inst.h"
#include "var.h"

LFO::LFO(rhythm& ry, Var const& v) {
#define find(nm) v.dic.find(L#nm) != v.dic.end()
	if (find(type)) {
		auto& tmp = *v.dic.at(L"type");
		if (tmp.typ == L"num") { type = tmp.num; }
		else { nm_fun = tmp.str; user_shape = true; }
	}
#define get(nm) if(find(nm)) { nm = v.dic.at(L#nm)->num; }
	get(sheet_time);
#undef get

#define get(nm) if(find(nm)) { nm = rthm_num(ry, *v.dic.at(L#nm)); }
	get(freq);
#undef get
#undef find
}

#define tp (ry.tempo)
#define nbt (ry.n_beat)

double LFO::val(rhythm &ry, Inst& inst) const {
	double fr = freq.val(ry);
	double f = sheet_time ? fr * nbt / 2 : fr;
	double ph = modf(f * inst.t, 1);
	if (user_shape) {
		if (ry.funs.find(nm_fun) == ry.funs.end()) { return 0; }
		auto& ys = *ry.funs.at(nm_fun); return get_val(ph, ys);
	}
	switch (type) {
	case LFO_SQR: return ph > 0.5 ? 1 : -1;
	case LFO_SAW: return 2 * ph - 1;
	case LFO_TRI: return abs(2 - 4 * ph) - 1;
	case LFO_SIN: return sin(2 * PI * ph);
	} return 0;
}
