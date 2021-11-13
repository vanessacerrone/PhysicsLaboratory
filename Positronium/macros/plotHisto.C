#include <vector>
#include <string>
#include "gethisto.C"

#include "TChain.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TStyle.h"
#include "TLegend.h"

using namespace std;

double myfit(double* x, double* par){
    Double_t b = par[0];
    Double_t a = par[1];

    Double_t fitval = 0;

    fitval = (b * x[0] + a);

    return fitval;
}


void plotHisto(const string name_file) {

    Int_t palette[8] = {632,800,400,416,840,432,600,880};

    TCanvas* c = new TCanvas("c", "canvas", 800, 800);
    c->Divide(2, 2, 0.0005, 0.0005);

    slimport_data_t indata0,indata1,indata2,indata3;
    TFile *infile = new TFile(name_file.c_str());
    TTree *intree = (TTree*)infile->Get("acq_tree_0");

    TH1F** h = new TH1F*[4]; 
    
    
    TBranch* branches[4];

    
    Double_t a[4] = {-17.7045,-21.7267,-22.9221,-0.0201002};
    Double_t b[4] = {0.115652 ,0.114383,0.110317,0.0116122};


    ULong64_t* timetag[4] = { &indata0.timetag, &indata1.timetag,&indata2.timetag,&indata3.timetag};
    //UShort_t* qlong[4] = {indata0.qlong, indata1.qlong, indata2.qlong,indata3.qlong };
    //UShort_t* q[4] = {&indata0.qlong, &indata1.qlong, &indata2.qlong,&indata3.qlong };

    vector<double> entry(4);

    int pad_offset = 0;

    for (int i=0;i < 4;i++) {

        branches[i] = intree->GetBranch(Form("acq_ch%i", i));

        branches[i]->SetAddress(timetag[i]);

        h[i] = new TH1F(Form("h%d",i), Form("h%d",i),1000,0,1700); 

            } 


    for (int i=0;i < 4;i++)
    
       {
        
        for (int j=0; j < branches[i]->GetEntries(); j++) {

        branches[i]->GetEntry(j);

        if (i == 0) entry[i] = a[i] + indata0.qlong*b[i];
        if (i == 1) entry[i] = a[i] + indata1.qlong*b[i];
        if (i == 2) entry[i] = a[i] + indata2.qlong*b[i];
        if (i == 3) entry[i] = a[i] + indata3.qlong*b[i];


        h[i]->Fill(entry[i]);
        
        }

        c->cd((i+1)-pad_offset);

        gPad->SetLeftMargin(0.15);
        gStyle->SetOptStat(1111);
        gStyle->SetTitleY(0.975);
        h[i]->SetMinimum(1);
        h[i]->GetXaxis()->SetTitle("Energy [keV]");
        h[i]->GetXaxis()->SetLabelOffset(0.01);
        h[i]->GetXaxis()->SetLabelSize(0.035);
        h[i]->GetXaxis()->SetTitleSize(0.04);
        h[i]->GetXaxis()->SetTitleOffset(1.15);
        h[i]->GetYaxis()->SetTitle("Counts");
        h[i]->GetYaxis()->SetLabelOffset(0.008);
        h[i]->GetYaxis()->SetLabelSize(0.035);
        h[i]->GetYaxis()->SetTitleSize(0.04);
        h[i]->GetYaxis()->SetTitleOffset(1.5);
        h[i]->GetXaxis()->SetRangeUser(0, 1400);
        h[i]->GetYaxis()->SetRangeUser(0, 4000);
        h[i]->SetLineWidth(1);
        h[i]->Draw();
        
    }


    
}



