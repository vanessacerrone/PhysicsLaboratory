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
  auto legend = new TLegend(0.67,0.75,0.85,0.87);

  TH1F** h = new TH1F*[N_points]; 
  for (int i=0;i < N_points;i++) { h[i] = new TH1F(Form("h%d",i), Form("h%d",i),1026,0,16384); } 

  for (int j=0; j < N_points; j++) {

    h[j] = getHistoForChannelFromTree(filelist[j],3,1026,0,16384);
    //TH1F* h1 = (TH1F*)h->Clone("gr");

    //h[j]->SetMinimum(1);
    h[j]->GetXaxis()->SetTitle("ADC counts");
    h[j]->SetLineColor(palette[j]);
    h[j]->SetFillColor(palette[j]);
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

    //TLegend *legend = new TLegend(0.16, 0.63, 0.45, 0.91);
    int n = j *4;
    legend->AddEntry(h[j],Form("%i ns",n),"f");
    legend->Draw();

  }
    
}




//void run_analysis() {
//  
//  TGraphErrors *gr = plot_tac();
//  
//
//  TCanvas *c1 = new TCanvas("c1", "c1", 1080, 1020);
//  gPad->SetLeftMargin(0.12);
//  gStyle->SetOptStat(0000);
//  gStyle->SetTitleY(0.975);
//  gr->SetMarkerStyle(8);
//  gr->SetMarkerSize(0.8);
//  gr->SetMarkerColor(kBlack);
//  gr->SetTitle("Tac calibration");
//
//  gr->GetXaxis()->SetTitle("Delay [ns]");
//  gr->GetXaxis()->SetLabelOffset(0.01);
//  gr->GetXaxis()->SetLabelSize(0.04);
//  gr->GetXaxis()->SetTitleSize(0.04);
//  gr->GetXaxis()->SetTitleOffset(1.15);
//  
//  gr->GetYaxis()->SetTitle("ADC counts");
//  gr->GetYaxis()->SetLabelOffset(0.008);
//  gr->GetYaxis()->SetLabelSize(0.04);
//  gr->GetYaxis()->SetTitleSize(0.04);
//  gr->GetYaxis()->SetTitleOffset(1.6);
//
//  gr->GetXaxis()->SetRangeUser(-20, 50);
//  gr->GetYaxis()->SetRangeUser(-40, 4000);
//
//
//  TF1 *f1 = new TF1("f1",myfit,-10,50,2);
//  TFitResultPtr fit_result = gr->Fit(f1,"RS");
//  f1->SetLineWidth(3);
//  f1->SetLineColor(kRed);
//  fit_result->Print("V");
//  gStyle->SetOptFit(0);   
//
//  TLegend *l = new TLegend(0.15,0.7,0.4,0.87);
//  l->AddEntry(gr,"Data points","p");
//  l->AddEntry(f1,"Function y = a + bx","l");    
//  
//  gr->Draw("ap");
//  f1->Draw("l SAME");
//  l->Draw();
//}
