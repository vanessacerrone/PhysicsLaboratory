/*
/	Get data from SaveHisto.cc output file 
/ and save calibrated histograms 
/  
/   11/20/2021  
*/


#include <iostream>
#include <string>
#include "RootStyle.cc"

void save_cal(string filename, int chan, string outfilename)
{
  set_root_style(1);

  TFile *infile = new TFile(filename.c_str());

  string histogram_name = "h_calib_ch_" + to_string(chan);
  string histogram_title = "Calibrated Spectrum chan " + to_string(chan);

  TH1F *cal_spectrum;

  Int_t nbins;


  Double_t a[4] = {-9.6562,-21.7267,-22.9221,-0.0201002};
  Double_t b[4] = { 0.0640172 ,0.114383,0.110317,0.0116122};

  if(chan == 0)
  {

    cal_spectrum = (TH1F*) infile->Get("ch0");
    Int_t max_bin = cal_spectrum->GetNbinsX(); // This method returns the number of bins in x of the histogram
	  Double_t max_kev = cal_spectrum->GetBinCenter(max_bin) * b[chan] + a[chan];
	  cal_spectrum->GetXaxis()->SetLimits(a[chan],max_kev);

  }

  if(chan == 1)
  {

    cal_spectrum = (TH1F*) infile->Get("ch1");
    Int_t max_bin = cal_spectrum->GetNbinsX(); // This method returns the number of bins in x of the histogram
	  Double_t max_kev = cal_spectrum->GetBinCenter(max_bin) * b[chan] + a[chan];
	  cal_spectrum->GetXaxis()->SetLimits(a[chan],max_kev);

  }
  
  
  if(chan == 2)
  {

    cal_spectrum = (TH1F*) infile->Get("ch2");
    Int_t max_bin = cal_spectrum->GetNbinsX(); // This method returns the number of bins in x of the histogram
	  Double_t max_kev = cal_spectrum->GetBinCenter(max_bin) * b[chan] + a[chan];
	  cal_spectrum->GetXaxis()->SetLimits(a[chan],max_kev);

  }


  if(chan == 3)
  {

    cal_spectrum = (TH1F*) infile->Get("ch3");
    Int_t max_bin = cal_spectrum->GetNbinsX(); // This method returns the number of bins in x of the histogram
	  Double_t max_kev = cal_spectrum->GetBinCenter(max_bin) * b[chan] + a[chan];
	  cal_spectrum->GetXaxis()->SetLimits(a[chan],max_kev);

  }


  // Saving to file
  TFile * outfile = new TFile(outfilename.c_str(), "RECREATE");
  cal_spectrum->Write();
  outfile->Close();
  infile->Close();

  return ;
}