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


TGraphErrors* plot_tac() {

  int N_points = 8;
  double delay[8] = {0,4,8,12,16,32,36,40};

  double offset;
  TGraphErrors *gr_tac = new TGraphErrors(N_points);
  for (int j=0; j<N_points; j++) {
    TH1F* h = getHistoForChannelFromTree(filelist[j],3,1026,0,16384);
    TH1F* h1 = (TH1F*)h->Clone("gr");
    int binmax = h->GetMaximumBin();
    double x_max = h->GetXaxis()->GetBinCenter(binmax);
    
    if(j == 0) {offset = x_max;}

    gr_tac->SetPoint(j,delay[j],(x_max - offset));
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
  gr->SetMarkerSize(0.8);
  gr->SetMarkerColor(kBlack);
  gr->SetTitle("Tac calibration");

  gr->GetXaxis()->SetTitle("Delay [ns]");
  gr->GetXaxis()->SetLabelOffset(0.01);
  gr->GetXaxis()->SetLabelSize(0.04);
  gr->GetXaxis()->SetTitleSize(0.04);
  gr->GetXaxis()->SetTitleOffset(1.15);
  
  gr->GetYaxis()->SetTitle("ADC counts");
  gr->GetYaxis()->SetLabelOffset(0.008);
  gr->GetYaxis()->SetLabelSize(0.04);
  gr->GetYaxis()->SetTitleSize(0.04);
  gr->GetYaxis()->SetTitleOffset(1.6);

  gr->GetXaxis()->SetRangeUser(-20, 50);
  gr->GetYaxis()->SetRangeUser(-40, 4000);


  TF1 *f1 = new TF1("f1",myfit,-10,50,2);
  TFitResultPtr fit_result = gr->Fit(f1,"RS");
  f1->SetLineWidth(3);
  f1->SetLineColor(kRed);
  fit_result->Print("V");
  gStyle->SetOptFit(0);   

  TLegend *l = new TLegend(0.15,0.7,0.4,0.87);
  l->AddEntry(gr,"Data points","p");
  l->AddEntry(f1,"Function y = a + bx","l");    
  
  gr->Draw("ap");
  f1->Draw("l SAME");
  l->Draw();

}