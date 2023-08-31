#include "mix_pdf.h"

MixPDF::MixPDF(ptr<PDF> pdf0, ptr<PDF> pdf1, double p0) {
	pdf_item item0{ pdf0, p0 };
	pdf_item item1{ pdf1, 1 - p0 };
	pdfs = { item0, item1 };
}
void MixPDF::normalize() {
	double sum = 0;
	for (auto& pdf : pdfs) { sum += pdf.p; }
	for (auto& pdf : pdfs) { pdf.p /= sum; }
}

double MixPDF::Val(vec3 v) const {
	double out = 0;
	for (auto& pdf : pdfs) { out += pdf.p * pdf.pdf->Val(v); }
	return out;
}
vec3 MixPDF::Gen() const {
	double p = frnd(1);
	for (auto& pdf : pdfs) {
		p -= pdf.p;
		if (p <= 0) { return pdf.pdf->Gen(); }
	}
	return pdfs.back().pdf->Gen();
}
