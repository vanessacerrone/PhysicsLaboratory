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
    TH1F *h = (TH1F*)dataFile->Get(Form("ch%i",chan));
    //background
    TFile *bgFile = new TFile(bgFileName.c_str());
    TH1F *hbg = (TH1F*)bgFile->Get(Form("ch%i",chan));

    h->Add(hbg, -1.);
    h->Draw();
}