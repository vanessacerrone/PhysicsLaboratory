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

double exp(double* x, double* par){
    Double_t b = par[1];
    Double_t a = par[0];

    Double_t fitval = 0;

    fitval = (a*exp(x[0]*b));

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


TGraphErrors* plot_tac() {

  int N_points = 8;
  double delay[8] = {0,4,8,12,16,32,36,40};

  double offset;
  TGraphErrors *gr_tac = new TGraphErrors(N_points);
  for (int j=0; j<N_points; j++) {
    TH1F* h = getHistoForChannelFromTree(filelist[j],3,1026,0,16384);
    TH1F* h1 = (TH1F*)h->Clone("h1");
    int binmax = h->GetMaximumBin();
    double x_max = h->GetXaxis()->GetBinCenter(binmax);
    
    if(j == 0) {offset = x_max;}

    gr_tac->SetPoint(j,(x_max - offset),delay[j]);
    gr_tac->SetPointError(j,0.,0);
      
    cout << "j: " << j << " mean: " << x_max << "\t" << x_max << endl; 
    delete h;
      
  }
    return gr_tac;
}


void run_analysis() {
  
  TGraphErrors *gr = plot_tac();
  
  TCanvas *c1 = new TCanvas("c1", "c1", 1080, 1020);
  gPad->SetLeftMargin(0.12);
  gStyle->SetOptStat(0000);
  gStyle->SetTitleY(0.975);
  gr->SetMarkerStyle(8);
  gr->SetMarkerSize(1);
  gr->SetMarkerColor(kBlack);
  gr->SetTitle("TAC calibration fit");

  gr->GetXaxis()->SetTitle("ADC counts");
  gr->GetXaxis()->SetLabelOffset(0.01);
  gr->GetXaxis()->SetLabelSize(0.04);
  gr->GetXaxis()->SetTitleSize(0.04);
  gr->GetXaxis()->SetTitleOffset(1.15);
  
  gr->GetYaxis()->SetTitle("Delay [ns]");
  gr->GetYaxis()->SetLabelOffset(0.008);
  gr->GetYaxis()->SetLabelSize(0.04);
  gr->GetYaxis()->SetTitleSize(0.04);
  gr->GetYaxis()->SetTitleOffset(1.);

  gr->GetYaxis()->SetRangeUser(-5, 50);
  gr->GetXaxis()->SetRangeUser(-40, 4000);


  TF1 *f1 = new TF1("f1",myfit,-5,4000,2);
  TFitResultPtr fit_result = gr->Fit(f1,"RS");
  f1->SetLineWidth(1);
  f1->SetLineColor(kRed);
  fit_result->Print("V");
  gStyle->SetOptFit(0);   

  TLegend *l = new TLegend(0.15,0.7,0.4,0.87);
  l->AddEntry(gr,"Data points","p");
  l->AddEntry(f1,"Linear fit","l");    
  
  
  f1->Draw("l");
  gr->Draw("ap same");
  l->Draw();

}


void plot_tac(const string file_na) {

  TH1F* h = getHistoForChannelFromTree(file_na.c_str(),3,400,400,30000);
  TH1F* h1 = (TH1F*)h->Clone("h1");

  double a = -0.0201002;
  double b = 0.0116122;
 
  TCanvas* c3 = new TCanvas("c3","3 gammas decay - TAC spectrum",1080,1020);
  gPad->SetLeftMargin(0.12);
  gStyle->SetOptStat(0000);
  gStyle->SetTitleY(0.975);

  int max_bin = h1->GetNbinsX(); 
	float max = h1->GetBinCenter(max_bin)*b + a;
	h1->GetXaxis()->SetLimits(a,max);
  h1->SetMinimum(1);
  h1->GetXaxis()->SetTitle("time [ns], arbitrary zero");
  h1->GetXaxis()->SetLabelOffset(0.01);
  h1->GetXaxis()->SetLabelSize(0.04);
  h1->GetXaxis()->SetTitleSize(0.04);
  h1->GetXaxis()->SetTitleOffset(1.15);
  h1->GetYaxis()->SetTitle("Counts/0.015 ns");
  h1->GetYaxis()->SetLabelOffset(0.008);
  h1->GetYaxis()->SetLabelSize(0.04);
  h1->GetYaxis()->SetTitleSize(0.04);
  h1->GetYaxis()->SetTitleOffset(1.1);
  h1->GetXaxis()->SetRangeUser(120, 190);
  h1->GetYaxis()->SetRangeUser(0, 50);
  h1->SetTitleOffset(1); 
  h1->SetFillColorAlpha(kAzure-9,0.5);
  h1->SetLineWidth(1);

  //TF1 *f1 = new TF1("f1",exp,155,156,2);
  //TFitResultPtr fit_result = h1->Fit(f1,"RS");
  //f1->SetLineWidth(1);
  //f1->SetLineColor(kRed);
  //f1->SetParameters(1300,-8.20731);
  //fit_result->Print("V");
  //gStyle->SetOptFit(0);  

  h1->Draw();
  //f1->Draw("l SAME");



}