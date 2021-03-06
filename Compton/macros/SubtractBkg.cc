
/*
 * Author      : Aurora
 * Description : Subtract bkg from spectra 

 * Usage       : $ cd /path/to/root/files
 *               $ root
 *               # .L SubtractBkg.cc
 *               # subtract("datafile.root", "bkgfile.root", channel)
*/

#include <iostream>
#include <string>
#include "RootStyle.cc"


using namespace std;

void subtract(string dataFileName, string bgFileName, short chan)
{
    set_root_style(0);

    /*Reading histogram file from the Root file*/

    // data
    TFile *dataFile = new TFile(dataFileName.c_str());
    TH1F *hdata = (TH1F*)dataFile->Get(Form("ch%i",chan));
    // background
    TFile *bgFile = new TFile(bgFileName.c_str());
    TH1F *hbg = (TH1F*)bgFile->Get(Form("ch%i",chan));

    /* Find the correct renormalization for the background histo since we have different acquisition times  */
    TH1F* hbgConverted = (TH1F*)hbg->Clone("h");
    double temp;

    for (int i = 1; i<=hbg->GetXaxis()->GetNbins(); i++) 
    {
        temp = 2 * hbg->GetBinContent(i); 
        hbgConverted->SetBinContent(i, temp);
    }

    // set calibration function parameters
    double a;
    double b;

    // different calibrations according to the channels 

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

    // calibrating x-axis for data spectrum
    int max_binData = hdata->GetNbinsX(); 
	float max_kevData = hdata->GetBinCenter(max_binData)*b + a;
	hdata->GetXaxis()->SetLimits(a,max_kevData);

    // same for background
    int max_binBg = hbgConverted->GetNbinsX(); 
	float max_kevBg = hbgConverted->GetBinCenter(max_binBg)*b + a;
    hbgConverted->GetXaxis()->SetLimits(a,max_kevBg);

    // subtracting bin-to-bin the histogram of bkg from the data one
    hdata->Add(hbgConverted, -1.);
    hdata->GetYaxis()->SetRangeUser(0, 201);
    hdata->GetXaxis()->SetRangeUser(0, 600);

    float w;
    w = hdata->GetXaxis()->GetBinWidth(0);
    hdata->GetXaxis()->SetTitle("Energy [keV]");
    hdata->GetYaxis()->SetTitle(Form("Counts/%0.1f keV",w));
    hdata->SetStats(kFALSE);
    hdata->SetTitle("Background subtracted Detector spectrum");
    hdata->GetYaxis()->SetTitleOffset(1.4);
    hdata->Draw();
    hdata->Draw();


    // fit the 256 keV gaussian 

    TF1 *fit1 = new TF1("g1","gaus(0)+pol1(3)", 160, 250);

    fit1->SetLineStyle(1);
	fit1->SetLineWidth(3);
    fit1->SetLineColor(kBlue);
    fit1->SetParameters(1.62237e+02,2.05413e+02,+1.91097e+01, 450.217,-0.0488353);

    hdata->Fit(fit1,"R");

    double mean = fit1->GetParameter(1);
    double sigma = fit1->GetParameter(2);
    double min = mean-3*sigma;
    double max = mean+3*sigma;


    // double area =  hdata->Integral( hdata->GetXaxis()->FindBin(160),hdata->GetXaxis()->FindBin(250));
    // cout << area <<endl;
    // cout << min << max << endl; 

    TH1F *h1 = (TH1F*)hdata->Clone();
    h1->SetFillColorAlpha(kAzure-9,1);
    h1->GetXaxis()->SetRangeUser(160,250);
    h1->Draw("SAME");

    // get counts under the gaussian peak  
    double counts = (fit1->Integral(160,250))/hdata->GetBinWidth(0);
    std::cout << "counts under peak: " << counts << std::endl;

    TPaveText *pt = new TPaveText(0.1,0.1,0.2,0.2,"blNDC");
    pt->AddText("N_{peak} = 3835");
    pt->AddText("#mu = (208 #pm 1) keV");
    pt->SetFillColor(0);
    pt->SetTextAlign(22);
    pt->SetBorderSize(0);
    pt->Draw("same");
    
    // rende il titolo della finestra colorato
    TText *t = pt->GetLineWith("3835");
    t->SetTextColor(kBlue);
    TText *t2 = pt->GetLineWith("keV");
    t2->SetTextColor(kBlue);
 
}
