#include "RootStyle.cc"

using namespace std;

void sub_compton(string infile,double fit_min, double fit_max,double sub_min, double sub_max)
{

  //700,1900,105,700
  set_root_style(0);

  TFile *infile = new TFile(infile.c_str());
  TH1F *h_spectrum = (TH1F*) infile->Get("ch0");

  int max_bin =  h_spectrum->GetNbinsX(); 
  float max_kev = h_spectrum->GetBinCenter(max_bin)*0.063135 -6.59056;
  h_spectrum->GetXaxis()->SetLimits(-6.59056,max_kev);

  // Fit Compton continuum
  TF1 *lin = new TF1("f1", "[0]+[1]*x", fit_min, fit_max);
  h_spectrum->Fit("f1", "R0");

  double q = lin->GetParameter(0);
  double m = lin->GetParameter(1);

  // Subtract the background 
  Int_t bin_min = h_spectrum->GetXaxis()->FindBin(sub_min);
  Int_t bin_max = h_spectrum->GetXaxis()->FindBin(sub_max);

  for (size_t i = bin_min; i < bin_max; i++)
  {
    double current_bin = h_spectrum->GetBinContent(i);
    h_spectrum->SetBinContent(i,  current_bin - m*h_spectrum->GetBinCenter(i)- q );
  }

  // Fit 511keV peak
  TF1 *fit = new TF1("fit","gaus(0)+pol1(3)",450,560);
  fit->SetLineStyle(1);
  fit->SetLineColor(kBlue);
  fit->SetLineWidth(2);
  fit->SetParameter(1, 511); 
  fit->SetParameters(30000,511,17, 450.217,-0.0488353);
  h_spectrum->Fit(fit,"R");

  double mean = fit->GetParameter(1);
  double sigma = fit->GetParameter(2);
  double min = mean-3*sigma;
  double max = mean+3*sigma;

  // Get counts under 511 keV and total counts
  double A511_2 =  h_spectrum->Integral(h_spectrum->GetXaxis()->FindBin(min),h_spectrum->GetXaxis()->FindBin(max));
  cout<<A511_2<<endl;
  double Atot =  h_spectrum->Integral(h_spectrum->GetXaxis()->FindBin(1),h_spectrum->GetXaxis()->FindBin(max_kev));
  cout<<Atot<<endl;
  cout<<h_spectrum->GetEntries();

  float w;
  w = h_spectrum->GetXaxis()->GetBinWidth(0);
  h_spectrum->GetXaxis()->SetRangeUser(0,700);
  h_spectrum->GetXaxis()->SetTitle("Energy [keV]");
  h_spectrum->GetYaxis()->SetTitle(Form("Counts/%0.1f keV",w));
  h_spectrum->SetStats(kFALSE);
  h_spectrum->SetTitle("Background subtracted Tagger spectrum");
  h_spectrum->GetYaxis()->SetTitleOffset(1.4);
  h_spectrum->Draw();
 
  fit->Draw("same");

  // Draw area under 511 keV peak
  TH1F *h1c = (TH1F*)h_spectrum->Clone();
  h1c->SetFillColorAlpha(kAzure-9,1);
  h1c->GetXaxis()->SetRangeUser(min,max);
  h1c->Draw("SAME");

  
  TPaveText *pt = new TPaveText(0.1,0.1,0.2,0.2,"blNDC");
  pt->AddText("A_{511} = 138428");
  pt->AddText("A_{tot} = 397760");
  pt->SetFillColor(0);
  pt->SetTextAlign(22);
  pt->SetBorderSize(0);
  pt->Draw("same");
  
  TText *t = pt->GetLineWith("138");
  t->SetTextColor(kBlue);
  TText *t2 = pt->GetLineWith("760");
  t2->SetTextColor(kBlue);
 
}
