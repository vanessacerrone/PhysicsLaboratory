/*-- TAC spectrum analysis  *--/

/*
 * Author  : Vanessa
 * Description:  
 * Usage   : $ cd /path/to/macro
 *           $ root
 *           # .L TacPlot.C
 *           # plot_tac()     // plots all peaks correponding to different delays  
*/

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

vector<string> filelist = {

    "../day3/TAC-0.root",
    "../day3/TAC-4.root",
    "../day3/TAC-8.root",
    "../day3/TAC-12.root",
    "../day3/TAC-16.root",
    "../day3/TAC-32.root",
    "../day3/TAC-36.root",
    "../day3/TAC-40.root"

    };


void plot_tac() {
 
  int N_points = 8;
  // color palette 
  Int_t palette[8] = {632,800,400,416,840,432,600,880};

  TCanvas* c1 = new TCanvas("c1","TAC calibration",1080,1020);
  gPad->SetLeftMargin(0.12);
  gStyle->SetTitleY(0.975);

  TH1F** h = new TH1F*[N_points]; 

  for (int i=0;i < N_points;i++) { 
    h[i] = new TH1F(Form("h%d",i), Form("h%d",i),2000,0,20000);
    h[i] = getHistoForChannelFromTree(filelist[i],3,1026,0,20000);
   } 


  for (int j=0; j < N_points; j++) {
   
    h[j]->GetXaxis()->SetTitle("TAC channel [a.u.]");
    h[j]->SetLineColor(kBlack);
    h[j]->SetFillColorAlpha(palette[j],0.3);
    h[j]->GetXaxis()->SetLabelOffset(0.01);
    h[j]->GetXaxis()->SetLabelSize(0.04);
    h[j]->GetXaxis()->SetTitleSize(0.04);
    h[j]->GetXaxis()->SetTitleOffset(1.15);
    h[j]->GetYaxis()->SetTitle("Counts");
    h[j]->GetYaxis()->SetLabelOffset(0.008);
    h[j]->GetYaxis()->SetLabelSize(0.04);
    h[j]->GetYaxis()->SetTitleSize(0.04);
    h[j]->GetYaxis()->SetTitleOffset(1.3);
    h[j]->GetYaxis()->SetMaxDigits(3);
    gStyle->SetOptStat(0000); 
    h[j]->Draw("same");

    auto legend = new TLegend(0.67,0.75,0.85,0.87);
    int n = j*4;
    legend->AddEntry(h[j],Form("%i ns",n),"f");
    legend->SetNColumns(4);
    legend->Draw();

  }
    
}



