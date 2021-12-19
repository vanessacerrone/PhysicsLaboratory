#include "TSpectrum.h"
#include <iostream>
#include <string>
#include "RootStyle.cc"


  


void peaks(string dataFileName, string bgFileName, short chan) {

   set_root_style(1);


    TFile *dataFile = new TFile(dataFileName.c_str());
    TH1F *hdata = (TH1F*)dataFile->Get(Form("ch%i",chan));
    //background
    double conversion_factor_BG = (20)/20;
    TFile *bgFile = new TFile(bgFileName.c_str());
    TH1F *hbg = (TH1F*)bgFile->Get(Form("ch%i",chan));

    /* This will help us to find the corret renormalization of the background file since we have different duration*/
    TH1F* hbgConverted = (TH1F*)hbg->Clone("h");
    double temp;

    for (int i = 1; i<=hbg->GetXaxis()->GetNbins(); i++) 
    {
        temp = conversion_factor_BG * hbg->GetBinContent(i); //need to find the factor of conversion for bg
        hbgConverted->SetBinContent(i, temp);
    }


    
   npeaks = TMath::Abs(np);
   TH1F *h = new TH1F("h","test",500,0,1000);
   //generate n peaks at random
   Double_t par[3000];
   par[0] = 0.8;
   par[1] = -0.6/1000;
   Int_t p;
   for (p=0;p<npeaks;p++) {
      par[3*p+2] = 1;
      par[3*p+3] = 10+gRandom->Rndm()*980;
      par[3*p+4] = 3+2*gRandom->Rndm();
   }
   TF1 *f = new TF1("f",fpeaks,0,1000,2+3*npeaks);
   f->SetNpx(1000);
   f->SetParameters(par);
   TCanvas *c1 = new TCanvas("c1","c1",10,10,1000,900);
   c1->Divide(1,2);
   c1->cd(1);
   h->FillRandom("f",200000);
   h->Draw();
   TH1F *h2 = (TH1F*)h->Clone("h2");
   //Use TSpectrum to find the peak candidates
   TSpectrum *s = new TSpectrum(2*npeaks);
   Int_t nfound = s->Search(h,2,"",0.10);
   printf("Found %d candidate peaks to fit\n",nfound);
   //Estimate background using TSpectrum::Background
   TH1 *hb = s->Background(h,20,"same");
   c1->Update();
   if (np <0) return;

   //estimate linear background using a fitting method
   c1->cd(2);
   TF1 *fline = new TF1("fline","pol1",0,1000);
   h->Fit("fline","qn");
   //Loop on all found peaks. Eliminate peaks at the background level
   par[0] = fline->GetParameter(0);
   par[1] = fline->GetParameter(1);
   npeaks = 0;
   Float_t *xpeaks = s->GetPositionX();
   for (p=0;p<nfound;p++) {
      Float_t xp = xpeaks[p];
      Int_t bin = h->GetXaxis()->FindBin(xp);
      Float_t yp = h->GetBinContent(bin);
      if (yp-TMath::Sqrt(yp) < fline->Eval(xp)) continue;
      par[3*npeaks+2] = yp;
      par[3*npeaks+3] = xp;
      par[3*npeaks+4] = 3;
      npeaks++;
   }
   printf("Found %d useful peaks to fit\n",npeaks);
   printf("Now fitting: Be patient\n");
   TF1 *fit = new TF1("fit",fpeaks,0,1000,2+3*npeaks);
   //we may have more than the default 25 parameters
   TVirtualFitter::Fitter(h2,10+3*npeaks);
   fit->SetParameters(par);
   fit->SetNpx(1000);
   h2->Fit("fit");             
}