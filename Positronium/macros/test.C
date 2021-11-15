
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

    vector<Double_t> x2 = {42,4432,6729,10435,12441,12521};
    vector<Double_t> y2 ={39.4666666666667,3530.66666666667,6540.8,3810.13333333333,6542.93333333333,4959.0303030303};
    vector<Double_t> ey2 ={0.8110350040397,7.6710132606093,10.440945040241,7.9688281586580,10.442647599574,9.09123231748617};

    vector<Double_t> err = {1.9310357239042 ,0.173082429165373, 0.155163397833868, 0.076366345554941, 0.083937365160147, 0.0726078773060153};

    vector<Double_t> loss;
    for(int i=0; i<x2.size();i++) {
        double l = 100 *(x2[i] - y2[i])/x2[i];
        loss.push_back(l);
        cout << loss[i] << endl;
    }


    TGraphErrors *graph0 = new TGraphErrors(1, &x2[0], &loss[0], 0,&err[0]);
    TGraphErrors *graph1 = new TGraphErrors(1, &x2[1], &loss[1], 0,&err[1]);
    TGraphErrors *graph2 = new TGraphErrors(1, &x2[2], &loss[2], 0,&err[2]);
    TGraphErrors *graph3 = new TGraphErrors(1, &x2[3], &loss[3], 0,&err[3]);
    TGraphErrors *graph4 = new TGraphErrors(1, &x2[4], &loss[4], 0,&err[4]);
    TGraphErrors *graph5 = new TGraphErrors(1, &x2[5], &loss[5], 0,&err[5]);

  
    
    TGraphErrors *graph6 = new TGraphErrors(6, &x2[0], &loss[0],0,&err[0]);
    TCanvas* c3 = new TCanvas("c3","Dead time",1080,1020);
    
    //TF1 *fit = new TF1("fit", myFit, 0, 15000,1);
    //fit->SetParameter(0,6e-05);
    //graph6->Fit(fit, "R");
    

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
    mg->Add(graph6);
  
    graph0->SetMarkerColor(632);
    graph0->SetMarkerStyle(8);
    graph0->SetMarkerSize(1);

    graph1->SetMarkerColor(800);
    graph1->SetMarkerStyle(8);
    graph1->SetMarkerSize(1);

    graph2->SetMarkerColor(kViolet);
    graph2->SetMarkerStyle(8);
    graph2->SetMarkerSize(1);

    graph3->SetMarkerColor(416);
    graph3->SetMarkerStyle(8);
    graph3->SetMarkerSize(1);

    graph4->SetMarkerColor(432);
    graph4->SetMarkerStyle(8);
    graph4->SetMarkerSize(1);

    graph5->SetMarkerColor(kBlue);
    graph5->SetMarkerStyle(8);
    graph5->SetMarkerSize(1);

    graph6->SetLineStyle(3);
	graph6->SetLineWidth(1);

    mg->Draw("aepl");
    //graph6->Draw("ep");
    //fit->Draw("same");

    mg->SetTitle("Dead Time");
    mg->GetYaxis()->SetTitle("Lost events [%]");
    mg->GetXaxis()->SetTitle("Rate Scaler [Hz]");



    auto legend = new TLegend(0.1,0.7,0.48,0.9);
    legend->AddEntry(graph3,"D1","ep");
    legend->AddEntry(graph5,"D2","ep");
    legend->AddEntry(graph4,"D3","ep");
    legend->AddEntry(graph2,"D4","ep");
    legend->AddEntry(graph1,"D1 & D2","ep");
    legend->AddEntry(graph0,"D1 & D2 & D4","ep");
    legend->SetNColumns(2);
    legend->Draw("same");


    //TCanvas* c = new TCanvas("c","Dead time - Lost events",1080,1020);
    //TGraph *g = new TGraph(6, &x2[0], &loss[0]);
    //gPad->SetLeftMargin(0.12);
    //gStyle->SetOptStat(0000);
    //gStyle->SetTitleY(0.975);
    //g->GetXaxis()->SetTitle("Real rate [Hz]");
    //g->GetYaxis()->SetTitle("Lost events [%]");
    //g->SetMarkerColor(kRed);
    //g->SetMarkerStyle(8);
    //g->SetMarkerSize(1);
    //g->Draw("APE4");
  

}
