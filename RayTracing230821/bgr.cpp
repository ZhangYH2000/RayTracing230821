#include "bgr.h"
#include "cur.h"
#include "draw_str.h"
#include "draw_tile.h"
#include "hit_item.h"

#define phtsel (cur.hit_sel)
#define htsel (*phtsel)

Bgr::Bgr(Cur& cur) {
	w = 1630; h = 860;
	tl = { 170, 0 }; dep = -100000;
	black = tile(w, h, dcol{}, 255);

	Reset();
}
void Bgr::Reset() {
	cm = cam();
	cm.look = {};
	cm.r = 25; cm.scl = 14;
	cm.min_d = 1; cm.max_d = 1e3;
	cm.phi = 0.5; cm.calc();
}

#include "my_def.h"

void Bgr::set_cm(Cur& cur) {
	dvec dmsp = msp - msp_old;
	if (dragged_mid) {
		dragged_mid = msd[1];
		if (dragged_mid && cur.real_time) {
			if (phtsel) {
				double theta = -dmsp.x * 0.005;
				double phi = dmsp.y * 0.005;
				htsel.tsf =
					mat3::rot(cm.mt.tsp().v0(), phi) *
					mat3::rot(vec3(0, 0, 1), -theta) * htsel.tsf;
			}
			else {
				cm.theta += -dmsp.x * 0.005;
				cm.phi += dmsp.y * 0.005;
			}
		}
	}
	else { dragged_mid = msc(1) && wheeled; }
	if (dragged_right) {
		dragged_right = msd[2];
		if (dragged_right && cur.real_time) {
			if (phtsel) {
				htsel.p = cm.mt * (htsel.p - cm.p);
				htsel.p.z -= dmsp.y / (cm.scl * h) * htsel.p.y;
				htsel.p.x += dmsp.x / (cm.scl * h) * htsel.p.y;
				htsel.p = cm.mt.tsp() * htsel.p + cm.p;
			}
			else {
				cm.look = cm.mt * (cm.look - cm.p);
				cm.look.z += dmsp.y / (cm.scl * h) * cm.look.y;
				cm.look.x -= dmsp.x / (cm.scl * h) * cm.look.y;
				cm.look = cm.mt.tsp() * cm.look + cm.p;
			}
		}
	}
	else { dragged_right = msc(2) && wheeled; }
	if (wheeled && cur.real_time) { cm.scl = exp(log(cm.scl) + msw * 0.1); }
	cm.theta = modf(cm.theta, 2 * PI);
	cm.psi = modf(cm.psi, 2 * PI);
	cm.phi = clmp(cm.phi, -PI / 2 + 0.001, PI / 2 - 0.001);
	cm.calc();
}
void Bgr::render(Cur& cur) {
	draw_tile_raw(scr, tl, scr.rect(), black, black.rect());

	if (cur.show) {
		if (cur.scale) {
			rep(i, 0, w) rep(j, 0, h) {
				int x = tl.x + i;
				int y = tl.y + j;
				int sx = i * wf / w;
				int sy = j * hf / h;
				int dp = y * scr.w + x;
				int sp = sy * wf + sx;
				auto c = film[sp];
				if (cur.gamma) {
					c.x = sqrt(c.x);
					c.y = sqrt(c.y);
					c.z = sqrt(c.z);
				}
				scr.cols[dp] = (dcol)c;
			}
		}
		else {
			rep(i, 0, wf) rep(j, 0, hf) {
				int x = tlf().x + i;
				int y = tlf().y + j;
				int dp = y * scr.w + x;
				int sp = j * wf + i;
				auto c = film[sp];
				if (cur.gamma) {
					c.x = sqrt(c.x);
					c.y = sqrt(c.y);
					c.z = sqrt(c.z);
				}
				scr.cols[dp] = (dcol)c;
			}
		}
	}

	draw_str(scr, dscr, 999, dbstr,
		dcol(255), ft, tl + dvec(10, 10), w - 20, bgr.vp());
}

void Bgr::Update(Cur& cur) {
	hovered = (hvd == this);
	wheeled = (whd == this);
	set_cm(cur);
	render(cur);
}
void Bgr::PreUpdate(Cur& cur) {
	bool ok = dhv <= dep && insd(msp, vp());
	if (ok) { dhv = dep; hvd = this; }

	ok = dwh <= dep && insd(msp, vp());
	if (ok) { dwh = dep; whd = this; }
}
