
using namespace std;

//C++ LIBRARIES
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>
//ROOT LIBRARIES
#include "TROOT.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TMath.h"

double_t myFit(double_t *x, double_t *par) 
{
  double_t fitval = x[0]/(par[0]*x[0] + 1);
  return fitval;
}


void plot()
{   


    vector<Double_t> x = {10435,12441,12521,6729,4432,42};
    vector<Double_t> y ={3810.13333333333,6542.93333333333,4959.0303030303,6540.8,3530.66666666667,39.4666666666667};



    TGraph *graph0 = new TGraph(1, &x[0], &y[0]);
    TGraph *graph1 = new TGraph(1, &x[1], &y[1]);
    TGraph *graph2 = new TGraph(1, &x[2], &y[2]);
    TGraph *graph3 = new TGraph(1, &x[3], &y[3]);
    TGraph *graph4 = new TGraph(1, &x[4], &y[4]);
    TGraph *graph5 = new TGraph(1, &x[5], &y[5]);

    vector<Double_t> x2 = {42,4432,6729,10435,12441,12521};
    vector<Double_t> y2 ={39.4666666666667,3530.66666666667,6540.8,3810.13333333333,6542.93333333333,4959.0303030303};
    vector<Double_t> ey2 ={0.8110350040397,7.6710132606093,10.440945040241,7.9688281586580,10.442647599574,9.09123231748617};

    vector<Double_t> loss;
    for(int i=0; i<x2.size();i++) {
        double l = 100 *(x2[i] - y2[i])/x2[i];
        loss.push_back(l);
        cout << loss[i] << endl;
    }



    TGraphErrors *graph6 = new TGraphErrors(6, &x2[0], &y2[0],0,&ey2[0]);
    TCanvas* c3 = new TCanvas("c3","Dead time",1080,1020);
    
    TF1 *fit = new TF1("fit", myFit, 0, 15000,1);
    fit->SetParameter(0,6e-05);
    graph6->Fit(fit, "R");
    fit->SetLineStyle(3);
	fit->SetLineWidth(2);

    gPad->SetLeftMargin(0.12);
    gStyle->SetOptStat(0000);
    gStyle->SetTitleY(0.975);

    TMultiGraph* mg = new TMultiGraph();
    mg->Add(graph0);
    mg->Add(graph1);
    mg->Add(graph2);
    mg->Add(graph3);
    mg->Add(graph4);
    mg->Add(graph5);
    

    graph0->SetMarkerColor(kRed);
    graph0->SetMarkerStyle(8);
    graph0->SetMarkerSize(1);

    graph1->SetMarkerColor(kBlue);
    graph1->SetMarkerStyle(8);
    graph1->SetMarkerSize(1);

    graph2->SetMarkerColor(kGreen);
    graph2->SetMarkerStyle(8);
    graph2->SetMarkerSize(1);

    graph3->SetMarkerColor(kCyan);
    graph3->SetMarkerStyle(8);
    graph3->SetMarkerSize(1);

    graph4->SetMarkerColor(kViolet);
    graph4->SetMarkerStyle(8);
    graph4->SetMarkerSize(1);

    graph5->SetMarkerColor(kOrange);
    graph5->SetMarkerStyle(8);
    graph5->SetMarkerSize(1);

    mg->Draw("APE4");
    //graph6->Draw("ep");
    fit->Draw("same");

    mg->SetTitle("Dead Time");
    mg->GetYaxis()->SetTitle("Mesasured rate [Hz]");
    mg->GetXaxis()->SetTitle("Rate Scaler [Hz]");


    auto legend = new TLegend(0.1,0.7,0.48,0.9);
    legend->AddEntry(graph0,"D1","p");
    legend->AddEntry(graph1,"D2","p");
    legend->AddEntry(graph2,"D3","p");
    legend->AddEntry(graph3,"D4","p");
    legend->AddEntry(graph4,"D1 & D2","p");
    legend->AddEntry(graph5,"D1 & D2 & D4","p");
    legend->Draw("same");


    TCanvas* c = new TCanvas("c","Dead time - Lost events",1080,1020);
    TGraph *g = new TGraph(6, &x2[0], &loss[0]);
    gPad->SetLeftMargin(0.12);
    gStyle->SetOptStat(0000);
    gStyle->SetTitleY(0.975);
    g->GetXaxis()->SetTitle("Real rate [Hz]");
    g->GetYaxis()->SetTitle("Lost events [%]");
    g->SetMarkerColor(kRed);
    g->SetMarkerStyle(8);
    g->SetMarkerSize(1);
    g->Draw("APE4");
  

}
