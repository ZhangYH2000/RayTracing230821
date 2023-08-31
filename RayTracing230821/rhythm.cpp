#include "rhythm.h"
#include "clip.h"
#include "track.h"
#include "var.h"
#include "fx.h"

rhythm::rhythm(wstring const& nm) {
	FILE* f = wfopen(nm, L"rb");
	if (!f) { return; }
	int sz = 0;

	sp = 44100;
	n_beat = 8; 
	tempo = 128; vol = 1; transpose = 0;

	frdv(cmd); frd(sz);
	rep(i, 0, sz) {
		double v = 0; wstring nm;
		frdv(nm); frd(v);
		gl[nm] = msh<Var>(v);
	}

	frd(sz);
	rep(i, 0, sz) {
		wstring nm; frdv(nm);
		vector<double> vs; frdv(vs);
		funs[nm] = msh<vector<double>>(vs);
	}

	tks.clear();

	// 这个本来是读取鼓机的，由于历史原因在这里，肯定要清理。
	rep(i, 0, 12 * 8) {
		auto tmp = msh<Track>(*this, f);
	}

	frd(sz);
	rep(i, 0, sz) { tks.push_back(msh<Track>(*this, f)); }
	Execute(gl, Compile(cmd)); fclose(f);

#define get(nm) if (gl.find(L#nm) != gl.end()) { nm = gl.at(L#nm)->num; }
	get(tempo); get(vol); get(transpose); get(t_stop); 
#undef get
}

#define ry (*this)
#define sp (ry.sp)
#define nbt (ry.n_beat)

clip rhythm::generate() {
	// 这里应该是要 reset 的。
	clip tmp(sp);
	double dt = tempo / (2 * nbt * sp);
	t_next = t_play + dt;
	while (t_play < t_stop) {
		double out = 0;
		for (auto tk : tks) { out += vol * tk->Play(*this); }
		tmp.vs.push_back(out); 
		t_play = t_next; t_next = t_play + dt;
	} return tmp;
}

void rhythm::reset() {
	t_play = t_next = 0;
	for (auto& tk : tks) { 
		tk->insts.clear(); 
		for (auto fx : tk->fxs) { fx->reset(*this); }
	}
}
