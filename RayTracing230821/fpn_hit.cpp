#include "fpn_hit.h"
#include "ui.h"
#include "cur.h"
#include "hit_item.h"

#define cur (*(Cur*)&app)
#include "my_def.h"
#define hts (cur.hits)
#define ht (*hts[id])
#define phtsel (cur.hit_sel)
#define htsel (*phtsel)

RTbHit::RTbHit(int id) : id(id), RichTextbox(360, 200) {}
void RTbHit::Sync(App& app) { str = ht.tmp_cmd; }
void RTbHit::OnDone(App& app) const { ht.tmp_cmd = str; }
BtDelHit::BtDelHit(int id) : id(id), BtLan(70, L"bt_del_hit") {}
void BtDelHit::OnClick(App& app) { ht.del = true; }
BtUpdateHit::BtUpdateHit(int id) : id(id), BtLan(70, L"bt_update_hit") {}
void BtUpdateHit::OnClick(App& app) { ht.total_refresh(cur); }
CbHitSel::CbHitSel(int id) : id(id), CbLan(L"cb_hit_sel", true) {}
void CbHitSel::Sync(App& app) { checked = &ht == phtsel; };
void CbHitSel::Upload(App& app) const { phtsel = checked ? &ht : NULL; }
CbHitShow::CbHitShow(int id) : id(id), CbLan(L"cb_hit_show", true) {}
void CbHitShow::Sync(App& app) { checked = ht.show; };
void CbHitShow::Upload(App& app) const { ht.show = checked; }

ClyHit::ClyHit(int id) {
	vector<Control*> tmp;
	mkp(rtb)(id);
	mkp(bt_del)(id);
	mkp(bt_update)(id);
	mkp(cb_sel)(id);
	mkp(cb_show)(id);
	tmp = { &*bt_del, &*bt_update, &*cb_sel, &*cb_show };
	mkcl(clx);
	mkp(spy)(400);
	cs = { &*rtb, &*clx, &*spy };
}
void ClyHits::Update(App &app) {
	rep(i, mbs.size(), hts.size()) {
		mbs.push_back(msh<ClyHit>(i));
	}
	cs.clear();
	rep(i, 0, hts.size()) {
		cs.push_back(&*mbs[i]);
	}
	rep(i, hts.size(), mbs.size()) {
		mbs[i]->Discard(app);
	}
	CtrlListY::Update(app);
}

BtCrtHit::BtCrtHit() : BtLan(200, L"bt_crt_hit") {}
void BtCrtHit::OnClick(App& app) {
	hts.push_back(msh<hit_item>());
}

FPnHit::FPnHit(App& app) : FPnLan(app, 400, 600, L"fpn_hit") {
	vector<Control*> tmp;
	mkp(cly_hits)();
	mkp(bt_crt_hit)();
	tmp = { &*cly_hits, &*bt_crt_hit };
	mkcl(cly); c = &*cly; Init(app);
}
