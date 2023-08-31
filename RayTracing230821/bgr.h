#pragma once
#include "vec3.h"
#include "tile.h"

struct Cur;
struct Bgr {
	dvec tl;
	double dep = 0;
	tile black;
	int w = 0, h = 0;

	cam cm;
	int wf = 0, hf = 0;
	vector<col3> film;

	bool hovered = false;
	bool wheeled = false;
	bool dragged_mid = false;
	bool dragged_right = false;

	Bgr(Cur& cur);
	void Reset();
	drect vp() const { return { tl, w, h }; }
	dvec tlf() const { return tl + dvec(w - wf, h - hf) / 2; }

	void set_cm(Cur& cur);
	void render(Cur& cur);
	void Update(Cur& cur);
	void PreUpdate(Cur& cur);
};
