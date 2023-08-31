#include "pn_menu.h"
#include "ui.h"
#include "cur.h"
#include "fpn_global.h"
#include "fpn_hit.h"
#include "fpn_cmd.h"
#include "fpn_par.h"
#include "fpn_lang.h"
#include "fpn_about.h"

#define cur (*(Cur*)&app)
#include "my_def.h"
#define mkbt(nm) mkp(bt_##nm)(&*ui.fpn_##nm);

BtMute::BtMute() : Button(150) {}
void BtMute::Update(App& app) {
	txt = cur.mute ? loc(L"unmute") : loc(L"mute");
	Button::Update(app);
}
void BtMute::OnClick(App& app) { cur.mute = !cur.mute; }
BtShow::BtShow() : Button(150) {}
void BtShow::Update(App& app) {
	txt = cur.show ? loc(L"hide") : loc(L"show");
	Button::Update(app);
}
void BtShow::OnClick(App& app) { cur.show = !cur.show; }
BtRealTime::BtRealTime() : Button(150) {}
void BtRealTime::Update(App& app) {
	txt = cur.real_time ? loc(L"fast") : loc(L"slow");
	Button::Update(app);
}
void BtRealTime::OnClick(App& app) { 
	cur.real_time = !cur.real_time; 
	cur.refresh_mode(); 
}

PnMenu::PnMenu(App& app) : Panel(Panel::menu(app)) {
	vector<Control*> tmp;
	mkp(bt_mute)(); mkp(bt_show)(); mkp(bt_real_time)();
	mkbt(global); mkbt(hit);
	mkbt(cmd); mkbt(par);
	mkbt(lang); mkbt(about);
	tmp = { &*bt_mute, &*bt_show, &*bt_real_time, &*bt_global, 
		&*bt_hit, &*bt_cmd, &*bt_par, &*bt_lang, &*bt_about };
	mkcl(cly); c = &*cly; Init(app);
}
