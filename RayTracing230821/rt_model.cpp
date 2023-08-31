#include "rt_model.h"
#include "tri.h"
#include "matt.h"
#include "metal.h"
#include "glass.h"
#include "model.h"
#include "smoke.h"
#include "col.h"
#include "checker.h"
#include "bvh_node.h"
#include "light.h"
#include "image.h"
#include "mix.h"
#include "hit_list.h"
#include "var.h"


#define found(s) (dic.find(s) != dic.end())
#define getv(nm) if (found(L#nm)) { nm = dic.at(L#nm)->num; }

Model::Model(Var const& v) {
	auto dic = v.dic;
	if (!found(L"name")) { ts = msh<HitList>(); }
	auto nm = dic.at(L"name")->str;
	auto m = model(L"models/" + nm + L".3d");

	vector<ptr<Hittable>> hs;
	for (auto& g : m.geos) {
		for (auto& f : g.faces) {
			bool checker = false; getv(checker);
			double bright = 1; getv(bright);
			double p_scatter = 1; getv(p_scatter);
			double fuzz = 0; getv(fuzz);
			bool smoke = false; getv(smoke);
			bool glass = false; getv(glass);
			bool gel = false; getv(gel);
			bool light = false; getv(light);
			double weight = 0.7; getv(weight);
			ptr<Texture> tex = msh<Col>(f.c);
			if (f.use_tile) { tex = msh<Image>(msh<tile>(f.t), bright); }
			if (checker) {
				double scl_checker = 1; getv(scl_checker);
				col3 c0 = 0.2, c1 = 0.8;
				if (found(L"c0_checker")) { c0 = tv3(*dic.at(L"c0_checker")); }
				if (found(L"c1_checker")) { c1 = tv3(*dic.at(L"c1_checker")); }
				tex = msh<Checker>(scl_checker, c0, c1);
			}

			auto matt = msh<Matt>(tex); matt->p = weight;
			auto metal = msh<Metal>(tex, fuzz);
			ptr<Material> m = msh<Mix>(matt, metal, p_scatter);
			if (light) { m = msh<Light>(tex); }
			if (smoke || glass || gel) { m = NULL; }

			for (auto& t : f.tris) {
				hs.push_back(msh<Tri>(t, m));
			}
		}
	}

	// 如果只有一个或者没有。
	if (hs.empty()) {
		ts = msh<HitList>();
	} else if (hs.size() == 1) {
		ts = hs[0];
	} else {
		ts = msh<BVHNode>(hs);
	}
}

Model::Model(wstring const& nm) {
	auto m = model(nm);
	static int i = 0;
	vector<ptr<Hittable>> hs;
	for (auto& g : m.geos) {
		for (auto& f : g.faces) {
			ptr<Material> m = msh<Matt>(msh<Col>(f.c));
			if (f.use_tile){
				m = msh<Matt>(msh<Image>(msh<tile>(f.t), 0.9));
			}
			m = msh<Matt>(msh<Checker>(2, 0.2, 0.8));
			if (i == 1) {

			}

			else  if (i == 0) {
				m = msh<Light>(msh<Image>(msh<tile>(f.t), 0.8));
				
			}

			for (auto& t : f.tris) {
				hs.push_back(msh<Tri>(t, m));
			}
		}
		i++;
	}
	// 按道理要检查数目是否超过1.
	ts = msh<BVHNode>(hs);
}
