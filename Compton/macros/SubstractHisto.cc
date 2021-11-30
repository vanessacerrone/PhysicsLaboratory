#include <iostream>
#include <string>
#include "RootStyle.cc"

/*requested files: histogramOfData.root, histogramOfBackground.root, channel*/


void substract(string dataFileName, string bgFileName, short chan)
{
    set_root_style(1);

    /*Reading histogram file from the Root file*/

    //data
    TFile *dataFile = new TFile(dataFileName.c_str());
    TH1F *hdata = (TH1F*)dataFile->Get(Form("ch%i",chan));
    //background
    TFile *bgFile = new TFile(bgFileName.c_str());
    TH1F *hbg = (TH1F*)bgFile->Get(Form("ch%i",chan));

    /* This will help us to find the corret renormalization of the background file since we have different duration*/
    TH1F* hbgConverted = (TH1F*)hbg->Clone("h");
    double temp;

    for (int i = 1; i<=hbg->GetXaxis()->GetNbins(); i++) 
    {
        temp = 2 * hbg->GetBinContent(i); //need to find the factor of conversion for bg
        hbgConverted->SetBinContent(i, temp);
    }

    //set calibration function parameters
    double a;
    double b;

    //different calibrations according to the parameters
    if (chan == 0)
    {
        a = -6.59056;
        b = 0.063135;
    }
    else if (chan == 1)
    {
        a = -7.25507;
        b = 0.0547132;
    }
    else 
    {
        a = -10.8547;
        b = 0.0607387;
    }

    //calibrating x-axis for data spectrum
    int max_binData = hdata->GetNbinsX(); 
	float max_kevData = hdata->GetBinCenter(max_binData)*b + a;
	hdata->GetXaxis()->SetLimits(a,max_kevData);

    //same for background
    int max_binBg = hbgConverted->GetNbinsX(); 
	float max_kevBg = hbgConverted->GetBinCenter(max_binBg)*b + a;
    hbgConverted->GetXaxis()->SetLimits(a,max_kevBg);

    //substracting bin-to-bin the histogram of bg from the data one
    hdata->Add(hbgConverted, -1.);
    hdata->GetYaxis()->SetRangeUser(0, 201);
    hdata->GetXaxis()->SetRangeUser(0, 1500);
    hdata->Draw();



    /*We can now fit the 256 keV gaussian*/

    TF1 *fit1 = new TF1("g1","gaus(0)+pol1(3)", 160, 250);

    fit1->SetLineStyle(1);
	fit1->SetLineWidth(3);
    fit1->SetLineColor(kRed);
    fit1->SetParameters(1.62237e+02,2.05413e+02,1.91097e+01, 450.217,-0.0488353);

    hdata->Fit(fit1,"R");

    /* Now, in order to get the counts under the gaussian fit */

    double counts = (fit1->Integral(160,250))/hdata->GetBinWidth(1);
    std::cout << "counts under peak: " << counts << std::endl;

    
}

/*
                        Fit result of gaussian
    1  p0           1.24373e+02   6.76003e+00   2.15600e-02  -3.16930e-05
    2  p1           2.07696e+02   9.56076e-01   4.09354e-03  -7.93442e-04
    3  p2          -1.99514e+01   1.65372e+00   3.08414e-03   2.52045e-04
    4  p3           1.28123e+02   1.69099e+01   1.08323e-02  -8.55828e-05
    5  p4          -4.87374e-01   8.42461e-02   5.13017e-05  -2.47612e-02

*/