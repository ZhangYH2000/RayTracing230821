#include "cur.h"
#include "ui.h"
#include "bgr.h"
#include "par.h"
#include "tri.h"
#include "rt_model.h"
#include "sphere.h"
#include "medium.h"
#include "smoke.h"
#include "glass.h"
#include "col.h"
#include "matt.h"
#include "light.h"
#include "pass.h"
#include "image.h"
#include "model.h"
#include "hit_item.h"
#include "hit_list.h"
#include "clip.h"
#include "refract.h"



Cur::Cur() {
	wv.len_block = 16 * 5e-4;
	wv.n_safe = 128; wv.n_blocks = 512;
	w = 1800; h = 900; set_locale();
	print_console(L"正在加载字体...");
	App::Init();
	print_console(L"字体加载完成.", true);
	print_console(L"正在加载控件...");
	mkp(ui)(*this); ui->Init(*this);
	mkp(bgr)(*this); Reset();
	print_console(L"控件加载完成.", true);
	print_console(L"正在加载音乐...");
	mkp(cl0)(L"sound/bgm0.clip");
	mkp(cl1)(L"sound/bgm1.clip");
	print_console(L"音乐加载完成.", true);
}

#define cur (*this)
#include "my_def.h"

void Cur::Save(wstring const& nm) const {
	FILE* f = wfopen(nm, L"wb");
	if (!f) { return; }
	int sz = 0; fwtv(cmd); save_par(f);
	cm.save(f);
	sz = hits.size(); fwt(sz);
	for (auto h : hits) { h->save(f); }
	fclose(f); 
}
void Cur::Load(wstring const& nm) {
	FILE* f = wfopen(nm, L"rb");
	if (!f) { return; } Reset();
	int sz = 0; frdv(cmd); 
	tmp_cmd = cmd; load_par(f);
	cm = cam(f);
	frd(sz);
	rep(i, 0, sz) { 
		hits.push_back(msh<hit_item>(*this, f)); 
	}

	Execute(gl, Compile(cmd)); fclose(f);
}
void Cur::Reset() {
	gl.clear(); init_def_fun(); pars.clear();
	bgr.Reset(); hits.clear(); hit_sel = NULL;

	w_fast = h_fast = 1.0 / 6;
	w_slow = h_slow = 1;
	n_fast = 2000; n_slow = 15000;
	d_fast = 10; d_slow = 300;
	c0_back = col3(0.6);
	c1_back = col3(0.2, 0.4, 0.6);
	refresh_mode();
}

#define found(s) (dic.find(s) != dic.end())
#define getv(nm) if (found(L#nm)) { nm = dic.at(L#nm)->num; }

void Cur::set_cfg(ptr<Var> v) {
	auto& dic = v->dic;
	getv(w_fast); getv(w_slow); getv(h_fast); getv(h_slow);
	getv(n_fast); getv(n_slow); getv(d_fast); getv(d_slow);
	if (found(L"c0_back")) { c0_back = tv3(*dic.at(L"c0_back")); }
	if (found(L"c1_back")) { c1_back = tv3(*dic.at(L"c1_back")); }
	refresh_mode();
}
#undef getv
#define getv(nm) if (found(L#nm)) { cm.nm = dic.at(L#nm)->num; }
void Cur::set_cam(ptr<Var> v) {
	if (!real_time) { return; }
	auto& dic = v->dic;
	getv(theta); getv(phi); getv(psi); 
	getv(scl); getv(r); getv(min_d); getv(max_d);
	if (found(L"look")) { cm.look = tv3(*dic.at(L"look")); }
}
#undef getv
#define getv(nm) if (found(L#nm)) { nm = dic.at(L#nm)->num; }

void Cur::Update() {
	bgr.PreUpdate(*this);
	ui.PreUpdate(*this);
	basic_update();

	auto& dic = gl;
	getv(vol);

	if (hit_sel && hit_sel->del) { hit_sel = NULL; }
	hits.erase(remove_if(hits.begin(), hits.end(), 
		[](ptr<hit_item> h) { return h->del; }), hits.end());

	auto tmp = msh<HitList>();
	world = tmp;
	for (auto& h : hits) {
		if (!h->show) { continue; }
		h->refresh(*this);
		if (h->hit) { tmp->Add(h->hit); }
	}

	if (cl == NULL) {
		cl = frnd(1) < 0.5 ? &*cl0 : &*cl1;
	}
	if (cl->csp >= cl->n()) {
		cl->csp = 0;
		cl = NULL;
	}
	if (cl && !mute) { cl->play(wv.wvin); }
	
	if (kbc(L'B') && kbd[VK_CONTROL]) { scale = !scale; }
	if (kbc(L'G') && kbd[VK_CONTROL]) { gamma = !gamma; }
	if (kbc(L'S') && kbd[VK_CONTROL]) { show = !show; }
	if (kbc(L'M') && kbd[VK_CONTROL]) { mute = !mute; }
	RayTrace();

	bgr.Update(*this);
	ui.Update(*this);
}

void Cur::save_par(FILE* f) const {
	int sz = pars.size(); fwt(sz);
	for (auto p : pars) { p->save(f); }
}
void Cur::load_par(FILE* f) {
	int sz = 0; frd(sz);
	rep(i, 0, sz) { pars.push_back(msh<param>(f)); }
}
void Cur::init_def_fun() {
	auto f0 = [this](vector<ptr<Var>> const& in) {
		if (in.size() >= 1) { set_cam(in[0]); }
		return msh<Var>();
	};
	gl[L"set_cam"] = msh<Var>(f0);

	auto f1 = [this](vector<ptr<Var>> const& in) {
		if (in.size() >= 1) { set_cfg(in[0]); }
		return msh<Var>();
	};
	gl[L"set_cfg"] = msh<Var>(f1);
}
void Cur::basic_update() {
	title = loc(L"title");
	if (gl.find(L"dbstr") != gl.end()) { dbstr = gl[L"dbstr"]->str; }
	if (gl.find(L"update") != gl.end()) {
		auto f = gl[L"update"];  Execute(gl, f->procs);
	}

	pars.erase(remove_if(pars.begin(), pars.end(),
		[](ptr<param> p) { return p->del; }), pars.end());
	for (auto& p : pars) { gl[p->nm] = msh<Var>(p->val); }
}

#define wf (bgr.wf)
#define hf (bgr.hf)

void Cur::refresh_mode() {
	bad_sample = i = j = total = 0;
	wf = bgr.w * (real_time ? w_fast : w_slow);
	hf = bgr.h * (real_time ? h_fast : h_slow);
	wf = clmp(wf, 1, bgr.w - 1);
	hf = clmp(hf, 1, bgr.w - 1);
	bgr.film.clear();
	bgr.film.resize(wf * hf);
}
void Cur::RayTrace() {

	int d = real_time ? d_fast : d_slow;
	rep(k, 0, real_time ? n_fast : n_slow) {

		int dp = j * wf + i;

		vec2 p;
		p.x = i + frnd(-0.5, 0.5);
		p.y = j + frnd(-0.5, 0.5);
		p -= vec2(wf, hf) / 2;
		p /= hf;

		vec3 v(p.x, cm.scl, -p.y);
		v = cm.mt.tsp() * v;
		v = v.unit();

		col3 c = ray_col(ray(cm.p, v), d, *this);
		if (real_time) {
			double m = 0.8;
			bgr.film[dp] *= 1 - m;
			bgr.film[dp] += m * c;
		} else {
			if (isfinite(c.x) && isfinite(c.y) && isfinite(c.z)) {
				bgr.film[dp] *= total;
				bgr.film[dp] += c;
				bgr.film[dp] /= (total + 1);
			} else { ++bad_sample; }
		}

		if (++i == wf) { i = 0; ++j; }
		if (j == hf) { j = 0; ++total; }
	}
}
void Cur::Output(wstring const& nm) const {
	tile t(wf, hf);
	rep(i, 0, wf) rep(j, 0, hf) {
		int p = j * wf + i;
		auto c = bgr.film[p];
		if (cur.gamma) {
			c.x = sqrt(c.x);
			c.y = sqrt(c.y);
			c.z = sqrt(c.z);
		}
		t.cols[p] = (dcol)c;
		t.as[p] = 255;
	}
	t.save(nm);
}
