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

    gr->GetXaxis()->SetTitle("E [ADC counts]");
    gr->GetYaxis()->SetTitle("E [keV]");
    gr->GetXaxis()->SetRangeUser(100,20000);
    //gr->GetYaxis()->SetRangeUser(0,0.002);
    gr->SetTitle("");
    gr->SetMarkerColor(kBlack);
    gr->GetYaxis()->SetMaxDigits(4);
    TF1 *f1 = new TF1("f1",LinearFit,300,20000,2);

    TFitResultPtr fit_result = gr->Fit(f1,"RS");
    
    f1->SetLineWidth(1);
    f1->SetLineColor(kRed);
    gr->Draw("ap");
    f1->Draw("same");
    fit_result->Print("V");

	// Get the parameters
	double m = f1->GetParameter(1);
    double em = f1->GetParError(1);
    double eq = f1->GetParError(1);
	double q = f1->GetParameter(0);
   
	cout << "Slope = " << m << ", Intercept = " << q << endl;


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
    gr2->GetXaxis()->SetTitle("E [ADC counts]");
    gr2->GetYaxis()->SetTitle("E [keV]");
    gr2->Draw("ap");
    gr2->GetYaxis()->SetRangeUser(-0.8,0.8);
        
    // linea di zero per i residui
    TLine *line = new TLine(300, 0, 20000, 0);
    line->SetLineStyle(2);
    line->SetLineColor(kBlack);
    line->Draw("same");
    
}

