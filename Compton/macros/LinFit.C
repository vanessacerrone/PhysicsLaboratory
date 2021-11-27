#include <iostream>
#include <string>
#include "RootStyle.cc"

double_t LinearFit(double_t *x, double_t *par) 
{
   double_t fitval2 = par[0] + par[1]*x[0];
  return fitval2;
}


void fit() {
    set_root_style(1);

    //double x[4] = {8.13486e+03,1.96613e+04,2.00570e+04,2.21480e+04};  // tagger
    double x[4] = {9.30445e+03,2.21039e+04,2.29508e+04,2.48324e+04};    // scatterer
    double y[4] = {511,1173.228,1274.537,1332.492};
	double erry[4] = {0,0.007,0.003,0.004};

	TGraphErrors *gr = new TGraphErrors(4,x,y,0,erry);

	// Define a function which fits the points


	gr->Draw("ALP");
    TF1 *f1 = new TF1("f1",LinearFit,7000,30000,2);
    TFitResultPtr fit_result = gr->Fit(f1,"RS");
    f1->SetLineWidth(1);
    f1->SetLineColor(kRed);
    fit_result->Print("V");

	// Get the parameters
	double m = f1->GetParameter(1);
	double q = f1->GetParameter(0);

	cout << "Slope = " << m << ", Intercept = " << q << endl;

}

