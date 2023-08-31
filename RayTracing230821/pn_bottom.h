#pragma once
#include "panel.h"
#include "label.h"
#include "ctrl_list.h"

struct LbFPS : Label {
	LbFPS();
	void Sync(App& app) override;
};
struct LbInfo : Label {
	LbInfo();
	void Sync(App& app) override;
};

struct PnBottom : Panel {
	ptr<LbFPS> lb_fps;
	ptr<LbInfo> lb_info;
	ptr<CtrlListX> clx;
	PnBottom(App& app);
};
