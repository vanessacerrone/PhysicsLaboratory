
using namespace std;

// Useful ROOT & C++ libraries

#include "TCanvas.h"
#include "TROOT.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "TLegend.h"
#include "TArrow.h"
#include "TLatex.h"
#include "TGraph.h"
#include "TH2F.h"

#include <iostream>
#include <fstream>
#include <vector>


// defines function y = a + bx to fit data

double myfit(double* x, double* par){
    Double_t b = par[0];
    Double_t a = par[1];

    Double_t fitval = 0;

    fitval = (b * x[0] + a);

    return fitval;
}


double myfit2(double* x, double* par){
    Double_t b = par[0];

    Double_t fitval = 0;

    fitval = (b * x[0]);

    return fitval;
}

// The argument is a string variable which contains the name of a txt file

void linearfit(const string file_name) {
    
    // fit range defined by user
    const double XMIN1 = -10;
    const double XMAX1 = 50;
   
    // Data lines should be x,y,errx,erry
    TGraph *g = new TGraph(file_name.c_str());
    TCanvas *c1 = new TCanvas("c1", "c1", 1080, 1020);
    gPad->SetLeftMargin(0.12);
    gStyle->SetOptStat(0000);
    gStyle->SetTitleY(0.975);
    g->SetLineColor(kBlack);
    g->SetMarkerColor(kBlack);
    g->SetMarkerStyle(8);
    g->SetMarkerSize(0.8);
    g->SetTitle("Tac Calibration ");
    g->GetXaxis()->SetTitle("t [ns]");
    g->GetXaxis()->SetTitleSize(0.04);
    g->GetXaxis()->SetTickLength(0.02);
    g->GetYaxis()->SetTitle("ADC counts");
    g->GetYaxis()->SetTitleSize(0.04);
    g->GetYaxis()->SetTickLength(0.02);
    g->GetYaxis()->SetRangeUser(-100, 4000);
    g->GetXaxis()->SetRangeUser(-10, 50);
    g->Draw("ap");
    
    
    TF1 *f1 = new TF1("f1",myfit,XMIN1,XMAX1,2);
    f1->SetParNames("b","a");
    TFitResultPtr fit_result = g->Fit(f1,"RS");
    f1->SetLineWidth(3);
    f1->SetLineColor(kRed);
    f1->Draw("SAME");
    fit_result->Print("V");
    gStyle->SetOptFit(0);
    c1->Update();

   
    auto legend = new TLegend(0.11,0.7,0.3,0.9);
    legend->AddEntry(g,"Measures","p");
    legend->AddEntry("f1","Function y = a + bx","l");
    legend->Draw();
    

}


  
    
