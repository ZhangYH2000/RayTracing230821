#pragma once
#include "label.h"
#include "button.h"
#include "textbox.h"
#include "ctrl_list.h"
#include "float_panel.h"
#include "checkbox.h"

struct BtSave : BtLan {
	BtSave();
	void OnClick(App& app) override;
};
struct BtLoad : BtLan {
	BtLoad();
	void OnClick(App& app) override;
};
struct BtOutput : BtLan {
	BtOutput();
	void OnClick(App& app) override;
};
struct BtReset : BtLan {
	BtReset();
	void OnClick(App& app) override;
};

struct FPnGlobal : FPnLan {
	ptr<Textbox> tb_save;
	ptr<BtSave> bt_save;
	ptr<CtrlListX> clx_save;
	ptr<Textbox> tb_load;
	ptr<BtLoad> bt_load;
	ptr<CtrlListX> clx_load;
	ptr<Textbox> tb_output;
	ptr<BtOutput> bt_output;
	ptr<CtrlListX> clx_output;
	ptr<BtReset> bt_reset;
	ptr<CtrlListY> cly;
	FPnGlobal(App& app);
};
