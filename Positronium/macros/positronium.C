#include "../example_macros/gethisto.C"

/*
/
/ Perform linear background + gaussian fit  
/ for two peaks in the spectrum
/
*/

void gaussianfit(const string file_na,short chan){


    TCanvas* c1 = new TCanvas("c1");

	gStyle->SetTitleBorderSize(0);
	gStyle->SetOptStat("");
	gStyle->SetOptFit(1111);
	gStyle->SetTitleAlign(23);
    
    // Retrieve histogram from tree
    TH1F *h_na = getHistoForChannelFromTree(file_na.c_str(), chan,1026,0,16384);

    h_na->GetYaxis()->SetTitle("Entries");
	h_na->GetXaxis()->SetTitle("ADC counts");
	h_na->SetTitle("Spectrum");
	h_na->GetYaxis()->SetTitleSize(0.04);
	h_na->GetYaxis()->SetTitleOffset(1.25);
	h_na->GetXaxis()->SetTitleSize(0.04);
	h_na->GetXaxis()->SetTitleOffset(1.25);

    // Fit functions

    TF1 *g1 = new TF1("g1","gaus(0)+pol1(3)",4000,5200);
    TF1 *g2 = new TF1("g2", "gaus(0)+pol1(3)", 10000, 12000);

	g1->SetLineStyle(1);
	g1->SetLineWidth(2);
	g1->SetParameter(1, 4500);
    g1->SetParameters(4780,4567,170, 450.217,-0.0488353);

    g2->SetLineStyle(1);
	g2->SetLineWidth(2);
	g2->SetParameters(8700,11000,315, 450.217,-0.0488353);


    h_na->Fit(g1, "R");
    h_na->Fit(g2, "R");
    h_na->Draw();

    g1->Draw("SAME");
    g2->Draw("SAME");
 
    double mean1 = g1->GetParameter(1);
	double sigma1 = g1->GetParameter(2);
    double mean2 = g2->GetParameter(1);
	double sigma2 = g2->GetParameter(2);

    
}



