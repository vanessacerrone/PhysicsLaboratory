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
    TCanvas *c = new TCanvas("c", "c");

	TGraphErrors *gr = new TGraphErrors(filename.c_str());

    gr->GetXaxis()->SetTitle("E [ADC counts]");
    gr->GetYaxis()->SetTitle("E [keV]");
    gr->GetXaxis()->SetRangeUser(0,13000);
    //gr->GetYaxis()->SetRangeUser(-5, 1650);
    gr->SetTitle("Calibration fit - HPGe detector");
    gr->SetMarkerColor(kBlack);
    gr->GetYaxis()->SetMaxDigits(4);
    TF1 *f1 = new TF1("f1",LinearFit,0,2200,2);
    //f1->SetParameter(1,0.0881642);
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


    
}

