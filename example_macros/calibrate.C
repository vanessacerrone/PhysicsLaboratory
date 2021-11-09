#include "gethisto.C"
//
//IMPORTANT:
//Only use axis rescaling if you have the histogram but not the data to generate it!!
//
void CalibrateHisto(const string file_na, float m, float q,short chan) { //Re-scaling of axis, as in the slides
	TH1F* h_uncal = getHistoForChannelFromTree(file_na.c_str(), chan,1026,0,16384);
	int max_bin = h_uncal->GetNbinsX(); // This method returns the number of bins in x of the histogram
	float max_kev = h_uncal->GetBinCenter(max_bin)*m + q;
	h_uncal->GetXaxis()->SetLimits(q,max_kev);
	if (m!=1 && q!=0) //This means that I actually changed the calibration!
	    h_uncal->SetXTitle("keV");
	 h_uncal->Draw();
}

//oid calibrate(const string file_na, short chan) {
//
//	// canvas
//	TCanvas *c0 = new TCanvas("c0");
//
//	// read the histos
//	TH1F* h_na = getHistoForChannelFromTree(file_na.c_str(), chan,1026,0,16384);
//
//	// search the peaks
//	TSpectrum *s = new TSpectrum(30);
//	int nPeaks;
//	double *na_bin;
//	nPeaks = s->Search(h_na,1,"",0.1);
//	na_bin = s->GetPositionX();
//
//	// fit the peaks with the Na energies
//	double na_kev[2]; 
//	na_kev[0] = 511.; 
//	na_kev[1] = 1275.;
//	TGraphErrors *graphErr = new TGraphErrors(nPeaks,na_bin,na_kev);
//	TF1 *fitfun = new TF1("calfitfun1","pol1",na_bin[0],na_bin[1]);
//	graphErr->Fit(fitfun);
//
//	// get the fit parameters
//	float m = fitfun->GetParameter(1);
//	float q = fitfun->GetParameter(0);
//
//	// call the function to calibrate the histogram
//	CalibrateHisto(h_na,m,q);
//
//	h_na->Draw();
//
//}

