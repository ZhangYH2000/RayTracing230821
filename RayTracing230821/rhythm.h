#pragma once
#include "utils.h"

struct Var;
typedef map<wstring, ptr<Var>> Scope;
struct clip;
struct Track;
struct rhythm {
	Scope gl;
	wstring cmd;
	int sp = 0; 
	int n_beat = 0;
	double vol = 0, tempo = 0, transpose = 0;
	double t_play = 0, t_stop = 0;
	double t_next = 0;
	vector<ptr<Track>> tks;
	map<wstring, ptr<vector<double>>> funs;

	rhythm(wstring const& nm);
	clip generate();
	void reset();
};
