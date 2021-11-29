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

    TH1F* hbgConverted = (TH1F*)hbg->Clone("h");
    double temp;
    for (int i = 1; i<=hbg->GetXaxis()->GetNbins(); i++) 
    {
        temp = 1 * hbg->GetBinContent(i); //need to find the factor of conversion for bg
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
    hdata->Draw();
}