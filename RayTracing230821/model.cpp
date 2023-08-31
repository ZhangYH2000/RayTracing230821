#include "model.h"
#include "var.h"

model::model(wstring const& nm) {
	FILE* f = wfopen(nm, L"rb");
	if (!f) { return; }
	int sz = 0; init_fun();

	cm = cam(f);
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
		auto fun = [vs](vector<ptr<Var>> const& in) {
			if (in.empty()) { return msh<Var>(); }
			return msh<Var>(get_val(in[0]->num, vs));
		};
		gl[nm] = msh<Var>(fun);
	}

	frd(sz);
	rep(i, 0, sz) { geos.push_back(geo(*this, f)); }
	Execute(gl, Compile(cmd));
	refresh(); fclose(f);
}
void model::refresh() {
	for (auto& g : geos) { g.refresh(*this); }
}

vector<tri> model::get_face(wstring const& nm) const {
	return faces.find(nm) == faces.end() ? vector<tri>{} : *faces.at(nm);
}
void model::save_face(ptr<Var> nm, ptr<Var> ts) {
	vector<tri> f;
	for (auto& v : ts->vec) {
		tri t; auto& tri = v->vec;
		if (tri.size() < 3) { continue; }
		t.va = tv3(*tri[0]);
		t.vb = tv3(*tri[1]);
		t.vc = tv3(*tri[2]);
		if (tri.size() >= 6) {
			t.ua = tv2(*tri[3]);
			t.ub = tv2(*tri[4]);
			t.uc = tv2(*tri[5]);
		}
		if (tri.size() >= 9) {
			// 注意这里有单位化。
			t.na = tv3(*tri[6]).unit();
			t.nb = tv3(*tri[7]).unit();
			t.nc = tv3(*tri[8]).unit();
		} 
		f.push_back(t);
	}
	faces[nm->str] = msh<vector<tri>>(f);
}
void model::init_fun() {
	auto save = [this](vector<ptr<Var>> const& in) {
		if (in.size() >= 2) { save_face(in[0], in[1]); }
		return msh<Var>();
	};
	gl[L"save"] = msh<Var>(save);
}
