
#include <iostream>
#include <random>
#include <chrono>
#include "TROOT.h"
#include "TMath.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TGraphErrors.h"
#include "TFitResult.h"
#include "TMatrixDSym.h"
#include <math.h>
#include "RootStyle.cc"

double fit_hurtado(double *x, double *par)
{
    double a1 = par[0]*TMath::Exp(-par[1]*pow(x[0],par[2]));
    double a2 = TMath::Exp(-par[3]*pow(x[0], par[4]));
    double a3 = TMath::Exp(-par[5]*pow(x[0], par[6]));

    return (( a1 + a2 ) * ( 1 - a3 ));
}
double fit_hurtado2(double *x, double *par)
{
    double a1 = par[0]*TMath::Exp(-par[1]*pow(x[0],par[2]));
    double a2 = TMath::Exp(-par[3]*pow(x[0], par[4]));
    double a3 = TMath::Exp(-par[5]*pow(x[0], par[6]));

    return par[7]*(( a1 + a2 ) * ( 1 - a3 ));
}
double fitzero2(double *x, double *par)
{
  double fitval = par[0] + par[1]*0;

  return fitval;
}

void eff() {
  set_root_style(1);

  //TCanvas * c1 = new TCanvas("c1", "efficiency curve ", 27, 50, 1020, 760);

  //this will be used for the fit
  TGraphErrors *g = new TGraphErrors("effdata.txt");
  


  g->SetName("fit");
  g->SetMarkerStyle(8);
  g->SetMarkerSize(0.9);
  g->SetMarkerColor(1);

  g->SetTitle("Efficiency curve for HPGe detector");
  g->GetYaxis()->SetTitle("Efficiency ");
  g->GetXaxis()->SetTitle(" E [keV] ");

  TF1 *f1 = new TF1("f1", fit_hurtado, 40, 1700, 7);
  
  f1->SetParameter(0,-0.5);
  f1->SetParameter(1,-9);
  f1->SetParameter(2,-0.2);
  f1->SetParameter(3,-500);
  f1->SetParameter(4,-1.2);
  f1->SetParameter(5,-0.2);
  f1->SetParameter(6,-0.22);
  //f1->SetParameter(7,1);

  f1->SetParNames("par0", "par1", "par2", "par3", "par4", "par5", "par6");

  g->Fit("f1","M","",40,1500);
  g->Fit("f1","M","",40,1500);
  g->Fit("f1","E M","",40,1500);
  g->Fit("f1","E M","",40,1500);
  g->Fit("f1","E M","",40,1500);
  
  //g->Draw("AP");
  //f1->Draw("SAME");
  

  double p0= f1->GetParameter(0);
  double p1 = f1->GetParameter(1);
  double p2= f1->GetParameter(2);
  double p3 = f1->GetParameter(3);
  double p4= f1->GetParameter(4);
  double p5 = f1->GetParameter(5);
  double p6= f1->GetParameter(6);
  //double p7=f1->GetParameter(7);

  double ep0 = f1->GetParError(0);
  double ep1 = f1->GetParError(1);
  double ep2 = f1->GetParError(2);
  double ep3 = f1->GetParError(3);
  double ep4 = f1->GetParError(4);
  double ep5 = f1->GetParError(5);
  double ep6 = f1->GetParError(6);

 TF1 *f2 = new TF1("f2", fit_hurtado2, 40, 1700, 8);
  //here I'm just cloning the function f1, modifying the 7th parameter with a 
  f2->SetParameter(0,p0);
  f2->SetParameter(1,p1);
  f2->SetParameter(2,p2);
  f2->SetParameter(3,p3);
  f2->SetParameter(4,p4);
  f2->SetParameter(5,p5);
  f2->SetParameter(6,p6);
  f2->SetParameter(7,1+1.7/100);
  TF1 *f3 = new TF1("f3", fit_hurtado2, 40, 1700, 8);
  //probably I'll need better parameters
  f3->SetParameter(0,p0);
  f3->SetParameter(1,p1);
  f3->SetParameter(2,p2);
  f3->SetParameter(3,p3);
  f3->SetParameter(4,p4);
  f3->SetParameter(5,p5);
  f3->SetParameter(6,p6);
  f3->SetParameter(7,1-1.7/100);

//f2->SetLineColor(kBlue);
//f2->SetFillStyle(3014);
//f2->SetFillColor(4);
//f2->Draw("SAME");

//f3->SetLineColor(kBlue);
//f3->SetFillStyle(3014);
//f3->SetFillColor(10);
//f3->Draw("SAME");

/*std::cout<<" val eff for 342: "<<f1->Eval(342)  <<"+-"<<f2->Eval(342)-f1->Eval(342)<<endl;
  std::cout<<" val eff for 245: "<<f1->Eval(245)  <<"+-"<<f2->Eval(245)-f1->Eval(245)<<endl;
  std::cout<<" val eff for 88: "<<f1->Eval(88)  <<"+-"<<f2->Eval(88)-f1->Eval(88)<<endl;
  std::cout<<" val eff for 575: "<<f1->Eval(575)  <<"+-"<<f2->Eval(575)-f1->Eval(575)<<endl;
  std::cout<<" val eff for 172: "<<f1->Eval(172)  <<"+-"<<f2->Eval(172)-f1->Eval(172)<<endl;*/
  
 



  /*std::cout<<" val eff for 342: "<<f1->Eval(342)  <<"+-"<<grint->GetErrorY(342)<<endl;
  std::cout<<" val eff for 245: "<<f1->Eval(245)  <<"+-"<<grint->GetErrorY(245)<<endl;
  std::cout<<" val eff for 88: "<<f1->Eval(88)  <<"+-"<<grint->GetErrorY(88)<<endl;
  std::cout<<" val eff for 575: "<<f1->Eval(575)  <<"+-"<<grint->GetErrorY(575)<<endl;
  std::cout<<" val eff for 172: "<<f1->Eval(172)  <<"+-"<<grint->GetErrorY(172)<<endl;*/


                                            /* COMPUTE RESIDUALS

  WARNING: if you're using this piece of code, every time you need to run it againg you have to 
  qui root in order to avoid crushes. (Idk wher's the problem).*/

  
  /*TCanvas* c2 = new TCanvas("c2", "residuisalita",
                             27, 50, 1020, 760);
  TGraphErrors* gr = (TGraphErrors*)g->Clone("gres");
  gr->SetTitle("residui");
  gr->GetXaxis()->SetTitle("f");
  gr->GetYaxis()->SetTitle("Residui");

   
 
 
  for (int i = 0; i < gr->GetN(); i++) {
        double x = gr->GetX()[i];
         double res = g->GetY()[i] - f1->Eval(x);
      gr->SetPoint(i, x, res);
      double ex = gr->GetEX()[i];
      double ey = gr->GetEY()[i];
      double eres2 = ey * ey + ex * ex * f1->Derivative(x) * f1->Derivative(x);
     //gr->SetPointError(i, 0, TMath::Sqrt(eres2)); 
      //std::cout<<res<<" "<<x<<" "<<eres2<<std::endl;
  }
TF1 *zero = new TF1("zero", fitzero2, 36, 1700, 2);
    zero->SetLineStyle(9);
    zero->SetLineColor(7);
   TMultiGraph* mg1 = new TMultiGraph();
    mg1->Add(gr);
    mg1->Draw("AP");
    mg1->SetTitle("Residuals Efficiency HPGe detector");
    mg1->GetYaxis()->SetTitle("Residuals ");
    mg1->GetXaxis()->SetTitle("E (keV)");
    zero->Draw("same");
    //c1->SaveAs("ploteff.pdf");*/
    

//Double_t peaks[10] = {77.1088,185.720,241.997,295.224,351.932,609.312,785.96,1120.287,1377.669,1764.494};
//Double_t peaks[4] = {77.1088,241.997,295.224,351.932};

//Double_t peaks[17] = {77.1088,185.720,241.997,295.224,351.932,463.002,510.74,583.108,609.312,768.356,911.196,934.34,968.960,1120.287,1238.111,1377.669,1764.494};

Double_t peaks[8] = {661.659,185.720,241.997,295.224,351.932,609.312,1120.287,1764.494};


for(unsigned int i = 0; i< 8; i++){
  cout << f1->Eval(peaks[i]) << endl;
}
cout << "------- NaI -------" << endl;
double val;
for(unsigned int i = 0; i< 8; i++){
  val = (-8.93124e+00* pow(peaks[i],-6.00311e-01 ))/((-1000*1.37583e-03 )+pow(peaks[i],-1.20989e+03 ));
  cout << val << endl;
}


}

