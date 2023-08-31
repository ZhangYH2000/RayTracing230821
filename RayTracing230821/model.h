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
	// ��Ϊ����Ҫ�õ��Լ���ָ�룬���ƺ��ƶ����Ϊ�����⡣
	// �Ժ�Ҫ��д�ģ������Ȼر�һ�°ɡ�
	void init_fun();
	vector<tri> get_face(wstring const& nm) const;
	void save_face(ptr<Var> nm, ptr<Var> ts);
};
