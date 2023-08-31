#include "reverb.h"
#include "rhythm.h"
#include "var.h"
#include "filter.h"

#define sp (ry.sp)

Comb::Comb(int n) : vs(n) {}
void Comb::reset() {
	for (auto& v : vs) { v = 0; }
}
double Comb::loop(double in, double g) {
	vs.push_back(in - g * vs.front());
	vs.pop_front(); return vs.back();
}

AllPass::AllPass(int n) : vs(n), ins(n) {}
void AllPass::reset() {
	for (auto& v : vs) { v = 0; }
	for (auto& v : ins) { v = 0; }
}
double AllPass::loop(double in, double g) {
	ins.push_back(in); ins.pop_front();
	vs.push_back(vs.front() * g + ins.front() - g * ins.back());
	vs.pop_front(); return vs.back();
}

Reverb::Reverb(rhythm &ry, Var const& v) {
	gc0 = 0.742;
	gc1 = 0.733;
	gc2 = 0.715;
	gc3 = 0.697;
	ga0 = 0.7;
	ga1 = 0.7;

	double tc0 = 0.0297;
	double tc1 = 0.0371;
	double tc2 = 0.0411;
	double tc3 = 0.0437;
	double ta0 = 0.09683;
	double ta1 = 0.03292;

#define find(nm) v.dic.find(L#nm) != v.dic.end()
#define get(nm) if(find(nm)) { nm = rthm_num(ry, *v.dic.at(L#nm)); }
	get(wet); get(gc0); get(gc1); get(gc2); get(gc3);
	get(ga0); get(ga1);
#undef get

	if (find(filter)) {
		auto& vs = v.dic.at(L"filter")->vec;
		for (auto v : vs) { fs.push_back(msh<Filter>(ry, *v)); }
	}

	c0 = Comb(sp * tc0);
	c1 = Comb(sp * tc1);
	c2 = Comb(sp * tc2);
	c3 = Comb(sp * tc3);
	a0 = AllPass(sp * ta0);
	a1 = AllPass(sp * ta1);

#undef find
}
void Reverb::reset(rhythm& ry) {
	c0.reset();
	c1.reset();
	c2.reset();
	c3.reset();
	a0.reset();
	a1.reset();
}
double Reverb::loop(rhythm& ry, double in) {
	double out =
		c0.loop(in, gc0.val(ry)) +
		c1.loop(in, gc1.val(ry)) +
		c2.loop(in, gc2.val(ry)) +
		c3.loop(in, gc3.val(ry));
	// 不知道要不要除以 4，我自作主张加的。
	out = a0.loop(out, ga0.val(ry));
	out = a1.loop(out, ga1.val(ry));
	for (auto f : fs) { out = f->loop(ry, out); }

	double wt = wet.val(ry);
	return wt * out + (1 - wt) * in;
}
