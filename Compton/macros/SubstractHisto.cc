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
        a = -7;
        b = 0.0631;
    }
    else if (chan == 1)
    {
        a = -7;
        b = 0.0547;
    }
    else 
    {
        a = -11;
        b = 0.0607;
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