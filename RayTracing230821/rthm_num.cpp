#include "rthm_num.h"
#include "rhythm.h"
#include "inst.h"
#include "env.h"
#include "lfo.h"
#include "var.h"

rthm_num::rthm_num(double num) : num(num) {}
rthm_num::rthm_num(rhythm& ry, Var const& v) {
	if (v.typ == L"num") { type = TYP_NUM; num = v.num; }
	else if (v.typ == L"vec") {
		auto& vs = v.vec; double bg = 0;
		if (vs.size() < 2) { return; }
		if (vs[0]->str == L"par") { 
			type = TYP_NUM; bg = 0;
			auto par = vs[1]->str;
			if (ry.gl.find(par) != ry.gl.end()) {
				num = ry.gl[par]->num; 
			}
		}
		if (vs[0]->str == L"env") { type = TYP_ENV; id = vs[1]->num; bg = 0; }
		if (vs[0]->str == L"lfo") { type = TYP_LFO; id = vs[1]->num; bg = -1; }
		if (vs.size() < 4) { return; }
		double c = vs[2]->num, d = vs[3]->num;
		a = (d - c) / (1 - bg); b = c - bg * a;

		// ������ܿ���д�ø��á�
		if (vs[0]->str == L"par") { num = a * num + b; }
	}
}

#define envs (inst.envs)
#define lfos (inst.lfos)

// ����һ����ʷ��������, ��Ϊ����û�� par���Ժ�Ҫ�ĵġ�
double rthm_num::val(rhythm& ry) const { return num; }
double rthm_num::val(rhythm& ry, Inst& inst) const {
	switch (type) {
	case TYP_NUM: return num;
	case TYP_ENV: return id < envs.size() ?
		a * envs[id]->val(ry, inst) + b : 0;
	case TYP_LFO: return id < lfos.size() ?
		a * lfos[id]->val(ry, inst) + b : 0;
	} return 0;
}
