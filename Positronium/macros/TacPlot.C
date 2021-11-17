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
  Int_t palette[8] = {632,800,400,416,840,432,600,880};

  TCanvas* c1 = new TCanvas("c1","TAC calibration",1080,1020);
  gPad->SetLeftMargin(0.12);
  gStyle->SetTitleY(0.975);
  auto legend = new TLegend(0.67,0.75,0.85,0.87);

  TH1F** h = new TH1F*[N_points]; 
  TH1F** h_new = new TH1F*[N_points]; 
  for (int i=0;i < N_points;i++) { h[i] = new TH1F(Form("h%d",i), Form("h%d",i),2000,0,20000);
    h[i] = getHistoForChannelFromTree(filelist[i],3,1026,0,20000);
    h_new[i] = new TH1F(Form("h_new%d",i), "TAC calibration",1000,4000,12000);
   } 


  for (int i=0; i < N_points; i++)  {

    int binmax = h[i]->GetMaximumBin();
    double x_max = h[i]->GetXaxis()->GetBinCenter(binmax);
    double std = h[i]->GetStdDev();
    double thr_up = x_max + 40;
    double thr_down = x_max - 10;

    for(int j =0; j< h[i]-> GetEntries();j++) {

      if( h[i]->GetXaxis()->GetBinCenter(j) < thr_up ) {
        h_new[i]->SetBinContent(j,h[i]->GetBinContent(j));
      }

      if ( (h[i]->GetXaxis()->GetBinCenter(j) > thr_up) && h[i]->GetMaximumBin() < 5000) {
        h_new[i]->SetBinContent(j,h[i]->GetBinContent(j));
        cout << h[i]->GetBinContent(j) << endl;
//
       }

    }
  }
  




  for (int j=0; j < N_points; j++) {
   
  
    h_new[j]->GetXaxis()->SetTitle("TAC channel [a.u.]");
    h_new[j]->SetLineColor(kBlack);
    h_new[j]->SetFillColorAlpha(palette[j],0.3);
    h_new[j]->GetXaxis()->SetLabelOffset(0.01);
    h_new[j]->GetXaxis()->SetLabelSize(0.04);
    h_new[j]->GetXaxis()->SetTitleSize(0.04);
    h_new[j]->GetXaxis()->SetTitleOffset(1.15);
    h_new[j]->GetYaxis()->SetTitle("Counts");
    h_new[j]->GetYaxis()->SetLabelOffset(0.008);
    h_new[j]->GetYaxis()->SetLabelSize(0.04);
    h_new[j]->GetYaxis()->SetTitleSize(0.04);
    h_new[j]->GetYaxis()->SetTitleOffset(1.3);
    h_new[j]->GetYaxis()->SetMaxDigits(3);
    gStyle->SetOptStat(0000); 
    //h[j]->Draw("same");
    h_new[j]->Draw("same");
    //TLegend *legend = new TLegend(0.16, 0.63, 0.45, 0.91);
    int n = j*4;
    legend->AddEntry(h_new[j],Form("%i ns",n),"f");
    legend->SetNColumns(4);
    legend->Draw();

  }
    
}



