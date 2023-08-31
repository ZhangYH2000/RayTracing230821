#pragma once
#include "app.h"
#include "var.h"

struct Sphere;
struct Hittable;
struct hit_item;

struct UI;
struct clip;
struct Bgr;
struct param;
struct Cur : App {
	ptr<UI> ui;
	ptr<Bgr> bgr;

	Scope gl;
	wstring dbstr, cmd, tmp_cmd;
	vector<ptr<param>> pars;

	int bad_sample = 0;
	int i = 0, j = 0, total = 0;
	int n_fast = 0, d_fast = 0;
	int n_slow = 0, d_slow = 0;
	double w_fast = 0, h_fast = 0;
	double w_slow = 0, h_slow = 0;
	bool real_time = true;

	ptr<clip> cl0, cl1;
	clip* cl = NULL;

	double vol = 1;
	bool mute = false;
	bool show = true;
	bool gamma = true;
	bool scale = true;

	col3 c0_back, c1_back;
	hit_item* hit_sel = NULL;
	vector<ptr<hit_item>> hits;
	ptr<Hittable> world;

	Cur();
	void Save(wstring const& nm) const;
	void Load(wstring const& nm);
	void Reset();
	void Update() override;

	void set_cfg(ptr<Var> v);
	void set_cam(ptr<Var> v);

	void save_par(FILE* f) const;
	void load_par(FILE* f);
	void init_def_fun();
	void basic_update();

	void refresh_mode();
	void RayTrace();
	void Output(wstring const& nm) const;
};
