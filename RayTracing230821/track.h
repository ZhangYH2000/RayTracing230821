#pragma once
#include "utils.h"

struct FX;
struct rhythm;
struct Inst;
struct nrect;
struct Track {
	wstring nm, str;
	ptr<Inst> inst;
	vector<ptr<Inst>> insts;
	vector<ptr<FX>> fxs;
	vector<vector<ptr<nrect>>> sht;

	Track(rhythm& ry, FILE* f);
	void add_nr(rhythm& ry, ptr<nrect> nr);
	double Play(rhythm& ry);
	void Build(rhythm& ry);
};
