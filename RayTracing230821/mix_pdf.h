#pragma once
#include "pdf.h"

struct pdf_item {
	ptr<PDF> pdf;
	double p = 0;
};

struct MixPDF : PDF {
	// 这个不能是空的。
	vector<pdf_item> pdfs;
	MixPDF(vector<pdf_item> const& pdfs) : pdfs(pdfs) { normalize(); }
	MixPDF(ptr<PDF> pdf0, ptr<PDF> pdf1, double p0);
	void normalize();

	double Val(vec3 v) const override;
	vec3 Gen() const override;
};