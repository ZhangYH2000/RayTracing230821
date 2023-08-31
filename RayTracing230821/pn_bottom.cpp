#include "pn_bottom.h"
#include "cur.h"
#include "bgr.h"

#define cur (*(Cur*)&app)
#include "my_def.h"

LbFPS::LbFPS() { w = 450; fixed_w = true; }
void LbFPS::Sync(App& app) {
	txt = loc(L"frm_tm") + tw2(cur.fps.frm_time * 1000) +
		L", FPS: " + tw2(cur.fps.fps);
}

LbInfo::LbInfo() { w = 450; fixed_w = true; }
void LbInfo::Sync(App& app) {
	if (cur.real_time) { txt = loc(L"not_started"); return; }
	double perc = 100.0 * (cur.i + bgr.wf * cur.j) / bgr.film.size();
	txt = loc(L"total_sample") + tw(cur.total) + L" (" + tw2(perc) +
		loc(L"bad_sample") + tw(cur.bad_sample);
}

PnBottom::PnBottom(App& app) : Panel(Panel::bottom(app)) {
	vector<Control*> tmp;
	mkp(lb_fps)(); mkp(lb_info)();
	tmp = { &*lb_fps, &*lb_info };
	mkcl(clx); c = &*clx; Init(app);
}
