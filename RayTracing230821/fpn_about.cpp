#include "fpn_about.h"
#include "cur.h"

#define cur (*(Cur*)&app)
#include "my_def.h"

FPnAbout::FPnAbout(App& app) : FPnLan(app, 800, 760, L"fpn_about") {
	vector<Control*> tmp;
	mkp(rtb_about)(760, L"about_txt");
	tmp = { &*rtb_about };
	mkcl(cly); c = &*cly; Init(app);
}
