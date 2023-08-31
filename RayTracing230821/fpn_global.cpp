#include "fpn_global.h"
#include "ui.h"
#include "cur.h"

#define cur (*(Cur*)&app)
#include "my_def.h"

BtSave::BtSave() : BtLan(80, L"bt_save") {}
void BtSave::OnClick(App& app) { cur.Save(ui.fpn_global->tb_save->str); }
BtLoad::BtLoad() : BtLan(80, L"bt_load") {}
void BtLoad::OnClick(App& app) { cur.Load(ui.fpn_global->tb_load->str); }
BtOutput::BtOutput() : BtLan(80, L"bt_output") {}
void BtOutput::OnClick(App& app) { cur.Output(ui.fpn_global->tb_output->str); }
BtReset::BtReset() : BtLan(200, L"bt_reset") {}
void BtReset::OnClick(App& app) { cur.Reset(); }

FPnGlobal::FPnGlobal(App& app) : FPnLan(app, 400, 600, L"fpn_global") {
	vector<Control*> tmp;
	mkp(tb_save)(270); mkp(bt_save)();
	tmp = { &*tb_save, &*bt_save }; mkcl(clx_save);
	mkp(tb_load)(270); mkp(bt_load)();
	tmp = { &*tb_load, &*bt_load }; mkcl(clx_load);
	mkp(tb_output)(270); mkp(bt_output)();
	tmp = { &*tb_output, &*bt_output }; mkcl(clx_output);
	mkp(bt_reset)();
	tmp = { &*clx_save, &*clx_load, &*clx_output, &*bt_reset };
	mkcl(cly); c = &*cly; Init(app);
}
