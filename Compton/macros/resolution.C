/*-- Plot peak precision as a function of # events -- */ 

#include <iostream>
#include <string>
#include "RootStyle.cc"


double_t myFit(double_t *x, double_t *par) 
{
   double_t fitval = par[0] + par[1]/sqrt(x[0]);
  return fitval;
}

void plot(string filename)
{
  set_root_style(1);
  // Retrieving data file
  TGraphErrors *g = new TGraphErrors(filename.c_str());
  TCanvas* c = new TCanvas("c","Precision on centroid",900,900);
  
  TF1 *f1 = new TF1("f1",myFit,600,22000,2);
  f1->SetParameters(0.04,5.7);
  TFitResultPtr fit_result = g->Fit(f1,"RS");
  f1->SetLineWidth(2);
  f1->SetLineColor(kRed);
  fit_result->Print("V");

  g->GetXaxis()->SetTitle("N entries");
  g->GetYaxis()->SetTitle("Precision [%]");
  g->SetTitle("Precision on 511 keV peak centroid");
  g->SetMarkerColor(kBlack);

  g->Draw("ap");
  f1->Draw("same");

  auto legend = new TLegend(0.67,0.75,0.85,0.87);
  legend->AddEntry(g,"Data points","p");
  legend->SetBorderSize(0);
  legend->AddEntry("f1","Fit y = a + b / #sqrt{N}","l");
  legend->Draw();


  return ;
}
