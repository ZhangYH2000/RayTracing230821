#pragma once
#include "geo.h"

struct Var;
typedef map<wstring, ptr<Var>> Scope;
struct model {
	cam cm;
	vector<geo> geos;
	map<wstring, ptr<vector<tri>>> faces;
	wstring cmd;
	Scope gl;
	
	model(wstring const &nm);
	void refresh();
	// 因为这里要用到自己的指针，复制和移动会成为大问题。
	// 以后要重写的，现在先回避一下吧。
	void init_fun();
	vector<tri> get_face(wstring const& nm) const;
	void save_face(ptr<Var> nm, ptr<Var> ts);
};
