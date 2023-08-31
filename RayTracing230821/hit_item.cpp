#include "hit_item.h"
#include "cur.h"
#include "var.h"
#include "sphere.h"
#include "translate.h"
#include "rt_model.h"
#include "rotate.h"
#include "medium.h"
#include "smoke.h"
#include "pass.h"
#include "glass.h"
#include "refract.h"
#include "sphere.h"
#include "var.h"

#include "my_def.h"

#define found(s) (dic.find(s) != dic.end())
#define getv(nm) if (found(L#nm)) { nm = dic.at(L#nm)->num; }

hit_item::hit_item(Cur & cur, FILE * f) {
	int sz = 0;
	frd(p); frd(tsf); frd(pdf); frdv(cmd);
	tmp_cmd = cmd;
	total_refresh(cur);
}
void hit_item::save(FILE* f) const {
	int sz = 0;
	fwt(p); fwt(tsf); fwt(pdf); fwtv(cmd);
}

void hit_item::total_refresh(Cur& cur) {
	cmd = tmp_cmd; 
	gl[L"ob"] = msh<Var>();
	Execute(gl, Compile(cmd));
	if (gl.find(L"ob") == gl.end()) { return; }

	auto dic = gl[L"ob"]->dic;
	wstring type;
	if (found(L"type")) { type = dic.at(L"type")->str; }
	if (type == L"sphere") {
		pre_hit = msh<Sphere>(*gl[L"ob"]);
	}
	else {
		pre_hit = msh<Model>(*gl[L"ob"]);
	}

	getv(pdf);
	bool smoke = false; getv(smoke);
	bool glass = false; getv(glass);
	bool gel = false; getv(gel);
	double fuzz = 0; getv(fuzz);
	double refract_idx = 1.5; getv(refract_idx);
	double density = 0; getv(density);
	double weight = 0.7; getv(weight);
	col3 c = 0.7;
	if (found(L"col")) { c = tv3(*dic.at(L"col")); }
	auto smk = msh<Smoke>(c); smk->p = weight;
	
	if (glass) {
		pre_hit = msh<Medium>(pre_hit, 
			msh<Glass>(c, fuzz), msh<Refract>(refract_idx), density);
	} else if (smoke) {
		pre_hit = msh<Medium>(pre_hit, smk, msh<Pass>(), density);
	} else if (gel) {
		pre_hit = msh<Medium>(pre_hit, 
			smk, msh<Refract>(refract_idx), density);
	}

	refresh(cur);
}
void hit_item::refresh(Cur& cur) {
	if (!pre_hit) { hit = {}; return; }
	
	hit = msh<Rotate>(pre_hit, tsf);
	hit = msh<Translate>(hit, p);

	// 以后应该需要解决物体本身参数与这里的 p, tsf 的竞争问题。
	// 但是这个的前提是 3D 编辑器能够真正组合多个几何体，并当成一个几何体看待。
}
