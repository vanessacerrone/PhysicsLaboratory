#include <iostream>
#include <string>
#include "RootStyle.cc"


using namespace std;

void plot(string dataFileName, string bgFileName, short chan){


    set_root_style(1);

    /*Reading histogram file from the Root file*/

    //data
    TFile *dataFile = new TFile(dataFileName.c_str());
    TH1F *hdata = (TH1F*)dataFile->Get(Form("ch%i",chan));
    //background
    TFile *bgFile = new TFile(bgFileName.c_str());
    TH1F *hbg = (TH1F*)bgFile->Get(Form("ch%i",chan));

    //set calibration function parameters
    double a;
    double b;

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


    TH1F* hbgConverted = (TH1F*)hbg->Clone("h");
    double temp;

    for (int i = 1; i<=hbg->GetXaxis()->GetNbins(); i++) 
    {
        temp = 2 * hbg->GetBinContent(i); 
        hbgConverted->SetBinContent(i, temp);
    }


    //calibrating x-axis for data spectrum
    int max_binData = hdata->GetNbinsX(); 
	float max_kevData = hdata->GetBinCenter(max_binData)*b + a;
	hdata->GetXaxis()->SetLimits(a,max_kevData);

    //same for background
    int max_binBg = hbgConverted->GetNbinsX(); 
	float max_kevBg = hbgConverted->GetBinCenter(max_binBg)*b + a;
    hbgConverted->GetXaxis()->SetLimits(a,max_kevBg);


    TCanvas* c1 = new TCanvas("c","c",900,900);

    //hdata->GetYaxis()->SetRangeUser(0, 2000);
    hdata->GetXaxis()->SetRangeUser(0, 2000);

    float w;
    w = hdata->GetXaxis()->GetBinWidth(0);
    hdata->GetXaxis()->SetTitle("Energy [keV]");
    hdata->GetYaxis()->SetTitle(Form("Counts/%0.1f keV",w));
    hdata->SetStats(kFALSE);
    hdata->SetTitle("Tagger spectrum");
    hdata->GetYaxis()->SetTitleOffset(1.4);
    hdata->Draw();
    

    TCanvas* c2 = new TCanvas("c2","c2",900,900);

    //hbgConverted->GetYaxis()->SetRangeUser(0, 2000);
    hbgConverted->GetXaxis()->SetRangeUser(0, 2000);

    float w1;
    w1 = hbgConverted->GetXaxis()->GetBinWidth(0);
    hbgConverted->GetXaxis()->SetTitle("Energy [keV]");
    hbgConverted->GetYaxis()->SetTitle(Form("Counts/%0.1f keV",w1));
    hbgConverted->SetStats(kFALSE);
    hbgConverted->SetTitle("Tagger background spectrum");
    hbgConverted->GetYaxis()->SetTitleOffset(1.4);
    hbgConverted->Draw();

}