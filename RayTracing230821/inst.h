#pragma once
#include "rthm_num.h"

struct rhythm;
struct Var;
struct Osc;
struct Env;
struct LFO;

struct Inst {
	int tone = 0;
	double t_end_rel = 0;
	double t_release = 0;

	// vol Ϊ����ʱ Inst �������� Osc ������ڹ�Ͳ�ͬ��ָ���� t_release��
	rthm_num vol;
	bool fixed_tone = false;

	double t = 0;
	bool end = false;
	vector<ptr<LFO>> lfos;
	vector<ptr<Env>> envs;
	vector<ptr<Osc>> oscs;
	
	Inst(rhythm& ry);
	Inst(Inst const& inst);
	void Set(int tn, double tr);
	double Sample(rhythm& ry);
};
