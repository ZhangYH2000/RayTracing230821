#include "track.h"
#include "inst.h"
#include "nrect.h"
#include "var.h"
#include "fx.h"
#include "rhythm.h"

Track::Track(rhythm &ry, FILE* f) {
	int sz = 0; frdv(nm); frdv(str);
	frd(sz); Build(ry);
	rep(i, 0, sz) { add_nr(ry, msh<nrect>(f)); }
}

#define nbt (ry.n_beat)
#define sp (ry.sp)
#define tp (ry.tempo)
#define t_play (ry.t_play)

void Track::add_nr(rhythm &ry, ptr<nrect> nr) {
	int id = max<int>(0, floor(nr->t0 / nbt));
	rep(i, sht.size(), id + 1) { sht.push_back({}); }
	sht[id].push_back(nr);
}

double Track::Play(rhythm& ry) {
	int a = max<int>(0, floor(t_play / nbt));
	int b = ceil(t_play / nbt) + 2;
	double c = tp / (2 * nbt);

	rep(i, a, min<int>(b, sht.size())) for (auto nr : sht[i])
	if (t_play <= nr->t0 && nr->t0 < ry.t_next) {
		auto tmp = msh<Inst>(*inst);
		tmp->Set(nr->tone, (nr->t1 - nr->t0) / c); insts.push_back(tmp);
	}

	double out = 0;
	for (auto inst : insts) if (!inst->end) { out += inst->Sample(ry); }
	for (auto f : fxs) { out = f->loop(ry, out); }
	insts.erase(remove_if(insts.begin(), insts.end(),
		[](ptr<Inst> inst) { return inst->end; }), insts.end()); return out;
}

#define gl (ry.gl)

void Track::Build(rhythm& ry) {
	insts.clear();
	fxs.clear(); 

	gl[L"fx"] = msh<Var>();
	gl[L"osc"] = msh<Var>();
	gl[L"lfo"] = msh<Var>();
	gl[L"env"] = msh<Var>();
	gl[L"vol"] = msh<Var>(1);
	gl[L"drum"] = msh<Var>(0);
	gl[L"fixed_tone"] = msh<Var>();
	Execute(gl, Compile(str));
	mkp(inst)(ry);

#define find(nm) gl.find(L#nm) != gl.end()
	if (find(fx)) {
		auto& vs = gl[L"fx"]->vec;
		for (auto v : vs) {
			fxs.push_back(msh<FX>(ry, *v));
		}
	}
#undef find

	gl[L"vol"] = msh<Var>(ry.vol);
}
