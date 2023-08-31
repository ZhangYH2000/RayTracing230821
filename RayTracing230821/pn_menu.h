#pragma once
#include "panel.h"
#include "button.h"
#include "ctrl_list.h"

struct BtMute : Button {
	BtMute();
	void Update(App& app) override;
	void OnClick(App& app) override;
};
struct BtShow : Button {
	BtShow();
	void Update(App& app) override;
	void OnClick(App& app) override;
};
struct BtRealTime : Button {
	BtRealTime();
	void Update(App& app) override;
	void OnClick(App& app) override;
};

struct PnMenu : Panel {
	ptr<BtMute> bt_mute;
	ptr<BtShow> bt_show;
	ptr<BtRealTime> bt_real_time;
	ptr<BtFPn> bt_global;
	ptr<BtFPn> bt_hit;
	ptr<BtFPn> bt_cmd;
	ptr<BtFPn> bt_par;
	ptr<BtFPn> bt_lang;
	ptr<BtFPn> bt_about;
	ptr<CtrlListY> cly;
	PnMenu(App& app);
};
