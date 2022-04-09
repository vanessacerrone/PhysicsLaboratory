/*-- Verify Inverse Square Law  *--/

/*
 * Description : Analysis of # events as a function of distance 
                 detector - source

 * Usage       : $ cd /path/to/txt/files
 *               $ root
 *               # .L distance.cxx
 *               # plot("file1.txt","file2.txt")  -> files for 511 and 1275 keV peaks 
 *               // txt files format: x (distance) y (N events) errx erry 
*/

#include <iostream>
#include <string>
#include "RootStyle.cc"


double_t myFit(double_t *x, double_t *par) 
{
   double_t fitval = par[0]/pow((x[0] + par[1]),2);
  return fitval;
}


void plot(string file511, string file1275)
{
  set_root_style(1);

  // Retrieving data file
  TGraphErrors *g = new TGraphErrors(file511.c_str());
  TGraphErrors *g2 = new TGraphErrors(file1275.c_str());
  TMultiGraph* mg = new TMultiGraph();
  TCanvas* c = new TCanvas("c","Precision on centroid",900,900);
  
  TF1 *f1 = new TF1("f1",myFit,15,55,2);
  TF1 *f2 = new TF1("f2",myFit,15,55,2);
  
  
  g->GetXaxis()->SetTitle("Distance [cm]");
  g->GetYaxis()->SetTitle("Number of entries");
  g->GetXaxis()->SetRangeUser(15,55);
  g->GetXaxis()->SetLimits(15,55);
  g->GetYaxis()->SetRangeUser(0,100000);
  g->GetYaxis()->SetLimits(0,100000);
  g->SetTitle("");

  g->SetMarkerColor(kRed);
  g2->SetMarkerColor(kBlue);
  

  f1->SetLineWidth(2);
  f1->SetLineColor(kRed);
  f1->SetLineStyle(7);
  g->Fit(f1,"R");

  f2->SetLineWidth(2);
  f2->SetLineColor(kBlue);
  f2->SetLineStyle(7);
  g2->Fit(f2,"R+");


  f2->Draw("al");
  f1->Draw("l same");
  g->Draw("aep");
  g2->Draw("ep");

  

  auto legend = new TLegend(0.67,0.75,0.85,0.87);
  legend->AddEntry(g,"511 keV data points","ep");
  legend->AddEntry(g2,"1275 keV data points","ep");
  legend->SetBorderSize(0);
  legend->Draw();


  TPaveText *pt = new TPaveText(0.1,0.1,0.2,0.2,"blNDC");
  //pt->AddText("y = a / (x + x_{0})^{2}");
  pt->AddText("a = (706 #pm  5) 10^{5} cm^{2}");
  pt->AddText("x_{0} = (6.9 #pm 0.1) cm");

  pt->SetFillColor(0);
  pt->SetBorderSize(0);
  pt->Draw("same");
  
  TPaveText *pt2 = new TPaveText(0.1,0.1,0.2,0.2,"blNDC");
  pt2->AddText("a = (167 #pm  2) 10^{5} cm^{2}");
  pt2->AddText("x_{0} = (6.7 #pm 0.2) cm"); 
  pt2->SetFillColor(0);
  pt2->SetBorderSize(0);
  pt2->Draw("same");

  TText *t = pt->GetLineWith("706");
  t->SetTextColor(kRed);
  TText *t1 = pt->GetLineWith("6.9");
  t1->SetTextColor(kRed);
  TText *t2 = pt2->GetLineWith("167");
  t2->SetTextColor(kBlue);
  TText *t3 = pt2->GetLineWith("6.7");
  t3->SetTextColor(kBlue);
  


  // calculate residuals
    TGraphErrors *gr = new TGraphErrors(file1275.c_str());
    for (int i=0; i<g2->GetN(); i++) {
      double res = g2->GetY()[i] - f2->Eval(g2->GetX()[i]); // residual
      gr->SetPoint(i,g2->GetX()[i],res);
      double eresy = g2->GetEY()[i];
      gr->SetPointError(i,0,eresy);
    }
    
    
    // new canvas
    TCanvas *c2 = new TCanvas("c2", "c2", 1080, 320);
    gr->SetTitle("");
    gr->SetMarkerColor(kBlack);
    gr->SetMarkerStyle(8);
    gr->SetMarkerSize(0.7);
    gr->SetLineWidth(1);
    gr->GetXaxis()->SetTitle("Distance [cm]");
    gr->GetYaxis()->SetTitle("y-y_{th}");
    gr->Draw("ap");
    // gr->GetYaxis()->SetRangeUser(-0.08,0.08);
        
    // linea di zero per i residui
    TLine *line = new TLine(15, 0, 55, 0);
    line->SetLineStyle(2);
    line->SetLineColor(kBlack);
    line->Draw("same");
    c2->Update();

}
