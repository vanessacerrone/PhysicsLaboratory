#include <iostream>
#include <string>
#include "RootStyle.cc"

double_t LinearFit(double_t *x, double_t *par) 
{
   double_t fitval2 = par[0] + par[1]*x[0];
  return fitval2;
}


void fit(const string filename) {
    set_root_style(1);
    TCanvas *c = new TCanvas("c", "c", 800, 600);

    TPad* upperPad = new TPad("lowerPad", "lowerPad",0,0.3,1,1);
    TPad* lowerPad = new TPad("upperPad", "upperPad", 0,0,1,0.3);
    upperPad->SetBottomMargin(0);
    lowerPad->SetTopMargin(0);
    upperPad->Draw(); 			       
    lowerPad->Draw(); 

    upperPad->cd();
	TGraphErrors *gr = new TGraphErrors(filename.c_str());

    gr->GetXaxis()->SetTitle("1 - cos(#theta)");
    gr->GetYaxis()->SetTitle("1/E_{f} [keV^{-1}]");
    //gr->GetYaxis()->SetRangeUser(0,0.002);
    gr->SetTitle("");
    gr->SetMarkerColor(kBlack);
    gr->GetYaxis()->SetMaxDigits(4);
	gr->Draw("ap");
    TF1 *f1 = new TF1("f1",LinearFit,300,20000,2);
    //f1->SetParameters(0.0019569472,0.0019569472);
    TFitResultPtr fit_result = gr->Fit(f1,"RS");
    
    f1->SetLineWidth(1);
    f1->SetLineColor(kRed);
    f1->Draw("same");
    fit_result->Print("V");

	// Get the parameters
	double m = f1->GetParameter(1);
    double em = f1->GetParError(1);
    double eq = f1->GetParError(1);
	double q = f1->GetParameter(0);
    double err = 1/(m*m) * em;
    double errq = 1/(q*q) * eq;
	cout << "Slope = " << m << ", Intercept = " << q << endl;
    cout << 1/m << "pm" << err <<endl;
    cout << 1/q << "pm" << errq <<endl;


    TPaveText *pt = new TPaveText(0.1,0.1,0.2,0.2,"blNDC");
    pt->AddText("m = (1791 #pm 8) 10^{-6} keV^{-1}");
    pt->AddText("q = (1927 #pm 6) 10^{-6} keV^{-1}");
    pt->AddText("#chi^{(2)}/ ndof = 1.9/3");
    pt->SetFillColor(0);
    pt->SetBorderSize(0);
    pt->Draw("same");


    // calculates residuals
    TGraphErrors *gr2 = new TGraphErrors(filename.c_str());
    for (int i=0; i<gr->GetN(); i++) {
      double res = gr->GetY()[i] - f1->Eval(gr->GetX()[i]); // residual
      gr2->SetPoint(i,gr->GetX()[i],res);
      double eresy = gr->GetEY()[i];
      gr2->SetPointError(i,0,eresy);
    }
    
    lowerPad->cd();
    
    gr2->SetTitle("");
    gr2->SetMarkerColor(kBlack);
    gr2->SetMarkerStyle(8);
    gr2->SetMarkerSize(0.7);
    gr2->SetLineWidth(1);
    gr2->GetXaxis()->SetTitle("1 - cos(#theta)");
    gr2->GetYaxis()->SetTitle("y-y_{th} [kev^{-1}]");
    gr2->Draw("ap");
    //gr2->GetYaxis()->SetRangeUser(-0.000025,0.000022);
        
    // linea di zero per i residui
    TLine *line = new TLine(0, 0, 1.1, 0);
    line->SetLineStyle(2);
    line->SetLineColor(kBlack);
    line->Draw("same");
    
}

