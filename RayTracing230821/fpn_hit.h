#pragma once
#include "button.h"
#include "ctrl_list.h"
#include "separator.h"
#include "rich_textbox.h"
#include "float_panel.h"
#include "checkbox.h"

struct RTbHit : RichTextbox {
	int id = 0;
	RTbHit(int id);
	void Sync(App& app) override;
	void OnDone(App& app) const override;
};
struct BtDelHit : BtLan {
	int id = 0;
	BtDelHit(int id);
	void OnClick(App& app) override;
};
struct BtUpdateHit : BtLan {
	int id = 0;
	BtUpdateHit(int id);
	void OnClick(App& app) override;
};
struct CbHitSel : CbLan {
	int id = 0;
	CbHitSel(int id);
	void Sync(App& app) override;
	void Upload(App& app) const override;
};
struct CbHitShow : CbLan {
	int id = 0;
	CbHitShow(int id);
	void Sync(App& app) override;
	void Upload(App& app) const override;
};

struct ClyHit : CtrlListY {
	ptr<RTbHit> rtb;
	ptr<BtDelHit> bt_del;
	ptr<BtUpdateHit> bt_update;
	ptr<CbHitSel> cb_sel;
	ptr<CbHitShow> cb_show;
	ptr<CtrlListX> clx;
	ptr<SeparatorY> spy;
	ClyHit(int id);
};
struct ClyHits : CtrlListY {
	vector<ptr<ClyHit>> mbs;
	void Update(App& app) override;
};
struct BtCrtHit : BtLan {
	BtCrtHit();
	void OnClick(App& app) override;
};
struct FPnHit : FPnLan {
	ptr<ClyHits> cly_hits;
	ptr<BtCrtHit> bt_crt_hit;
	ptr<CtrlListY> cly;
	FPnHit(App &app);
};
