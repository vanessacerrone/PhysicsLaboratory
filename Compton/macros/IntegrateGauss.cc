#include <iostream>
#include <string>
#include "RootStyle.cc"


void FitGauss(const string infilename,short chan)
{
    set_root_style(1);

    //  Reading histogram file from the Root file 
    TFile *infile = new TFile(infilename.c_str());
    TH1F *h = (TH1F*)infile->Get(Form("ch%i",chan));

    // Fitting the two gaussians 
    TF1 *fit1 = new TF1("g1","gaus(0)+pol1(3)", 470, 580);
    TF1 *fit2 = new TF1("g2", "gaus(0)+pol1(3)", 2150, 2430);
    //TF1 *fit_a = new TF1("ga", "gaus(0)+pol1(3)", 700, 1400);
    


    fit1->SetLineStyle(1);
	fit1->SetLineWidth(3);
    fit1->SetParameters(2.09543e+03, 5.24550e+02, 1.77987e+01 , 450.217,-0.0488353); //for D1,D2,D3
    //fit1->SetParameters(1.25784e+04,2.99596e+03,1.05122e+02, 450.217,-0.0488353); //forD4

    fit2->SetLineStyle(1);
	fit2->SetLineWidth(3);
    fit2->SetLineColor(kViolet);
	fit2->SetParameters(1.73807e+02,2.27391e+03,5.03210e+01, 450.217,-0.0488353); //for D1,D2,D3
    //fit2->SetParameters(2.62637e+03,7.19168e+03,2.07706e+02, 450.217,-0.0488353); //for D4

    //fit_a->SetLineStyle(1);
	//fit_a->SetLineWidth(3);
    //fit_a->SetLineColor(kBlue);
	//fit_a->SetParameters(2.72700e+03,1000,1.96384e+02, 450.217,-0.0488353);

    h->Fit(fit1,"R");
    h->Fit(fit2,"R+");
    //h->Fit(fit_a,"R+");

    // Get parameters 
    double mean_1 = fit1->GetParameter(1);
    double mean_2 = fit2->GetParameter(1);
    //double mean_a = fit_a->GetParameter(1);

    double sigma_1 = fit1->GetParameter(2);
    double sigma_2 = fit2->GetParameter(2);
    //double sigma_a = fit_a->GetParameter(2);

    double min1 = mean_1-3*sigma_1;
    double max1 = mean_1+3*sigma_1;

    double min2 = mean_2-3*sigma_2;
    double max2 = mean_2+3*sigma_2;
    // Setting histogram info 
    h->SetMinimum(1);
    h->GetXaxis()->SetTitle("ADC counts");
    h->GetYaxis()->SetTitle("Counts");


    // Setting canvas info  and drawing histogram
    TCanvas* c1 = new TCanvas("c1","Plot of not calibrated spectra",900,900);
    h->Draw();
    fit1->Draw("SAME");
    fit2->Draw("SAME");

    double area1 =  fit1->Integral(min1,max1);
    double area2 =  fit2->Integral(min2,max2);
    std::cout<<area1<<std::endl<<area2<<std::endl;

}
