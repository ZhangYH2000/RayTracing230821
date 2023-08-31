#include "fx.h"
#include "rhythm.h"
#include "var.h"
#include "reverb.h"

FX::FX(rhythm &ry, Var const& v) {
	if (v.vec.size() >= 2) {
		type = v.vec[0]->num;
		switch (type) {
		case FX_REVERB: mkp(reverb)(ry, *v.vec[1]); return;
		}
	}
}
void FX::reset(rhythm &ry) {
	if (reverb) { reverb->reset(ry); }
}
double FX::loop(rhythm& ry, double in) {
	switch (type) {
	case FX_REVERB: return reverb->loop(ry, in);
	} return 0;
}
