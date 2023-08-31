#include "filter.h"
#include "rhythm.h"
#include "var.h"

Filter::Filter(rhythm& ry, Var const& v) {
#define find(nm) v.dic.find(L#nm) != v.dic.end()
#define get(nm) if(find(nm)) { nm = v.dic.at(L#nm)->num; }
	get(type);
#undef get

#define get(nm) if(find(nm)) { nm = rthm_num(ry, *v.dic.at(L#nm)); }
	get(freq); get(resonance);
#undef get
#undef find
}

#define sp (ry.sp)

double Filter::loop(rhythm& ry, double in) {
	// 版本维护会是个问题啊。
	// 以后还是把这个改成归一化的算了。
	double f = 2 * sin(PI * freq.val(ry) / sp);
	double rs = resonance.val(ry);
	double fb = rs + rs * (1 - f);

	double hp = in - buf0;
	double bp = buf0 - buf1;
	buf0 = buf0 + f * (hp + fb * bp);
	buf1 = buf1 + f * (buf0 - buf1);

	switch (type) {
	case FLT_LP: return buf1;
	case FLT_HP: return hp;
	case FLT_BP: return bp;
	case FLT_ALT_LP: return buf0;
	case FLT_ALT_HP: return in - buf1;
	} return 0;
}


double Filter::loop(rhythm& ry, Inst& inst, double in) {
	// 版本维护会是个问题啊。
	// 以后还是把这个改成归一化的算了。
	double f = 2 * sin(PI * freq.val(ry, inst) / sp);
	double rs = resonance.val(ry, inst);
	double fb = rs + rs * (1 - f);

	double hp = in - buf0;
	double bp = buf0 - buf1;
	buf0 = buf0 + f * (hp + fb * bp);
	buf1 = buf1 + f * (buf0 - buf1);

	switch (type) {
	case FLT_LP: return buf1;
	case FLT_HP: return hp;
	case FLT_BP: return bp;
	case FLT_ALT_LP: return buf0;
	case FLT_ALT_HP: return in - buf1;
	} return 0;
}
