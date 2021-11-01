#include "gethisto.C"

void subtract_bg(const string file_na, const string file_bg) {

	// canvas
	TCanvas *c0 = new TCanvas("c0");

	TH1F *h_na = getHistoWithFilter(file_na.c_str(),514,0,16384,2000);
	TH1F *h_bg = getHistoWithFilter(file_bg.c_str(),514,0,16384,2000);
    TH1F *h_subtr = (TH1F*)h_na->Clone();
	h_subtr->Add(h_bg,-1);

	h_na->Draw();
	h_subtr->SetLineColor(2);
	h_subtr->Draw("SAME");
	c0->Print("subtr_bg.png");

}

