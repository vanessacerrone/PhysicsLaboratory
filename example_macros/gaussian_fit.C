#include "gethisto.C"

void gaussian_fit(const string file_na) {

	// Canvas
	TCanvas *c[3];
	for (int i=0; i<3; i++)
	   c[i] = new TCanvas(Form("c%i",i));

	// Get a histo with gaussian peaks
    TH1F *h_na = getHistoFromTree(file_na.c_str(),1026,0,16384);
	c[0]->cd();
	h_na->Draw();
	
	// Get the two peaks of Na
	TSpectrum *s = new TSpectrum(30); // parameter i  maximum number of peaks
	int nPeaks;
	double *xPeaks;
    nPeaks = s->Search(h_na,1,"goff",0.12); // (histo, sigma, option, threshold)
    // peaks with amplitude less than threshold*highest_peak are discarded
	xPeaks = s->GetPositionX();

	// I fit the two peaks with a Gaussian to get better values of centroids and sigmas

	int npoints;
	TH1F *h_fitted[2];
	float left_mk, right_mk, x_fit_min, x_fit_max, chisqr, mean, sigma;
	double apertura_sx = 250; // arbitrary values. Per un calcolo raffinato si può fare un secondo fit dove questi valori corrispondono alle sigma del primo fit...
	double apertura_dx = 450;
    
	for (int p=0; p<2; p++) {
		c[p+1]->cd(); // change canvas
		h_fitted[p] = (TH1F*)h_na->Clone(); // Clone on a new TH1F to show separately the fits
		x_fit_min = xPeaks[p] - left_mk; // fit left margin
		x_fit_max = xPeaks[p] + right_mk; // fit right margin
		// fit function
		TF1 *fit = new TF1("fit","gaus",x_fit_min,x_fit_max);
		fit->SetParameters(1,xPeaks[p]); // Fix the centroid from the already known x
		fit->SetNpx(1000); // Number of points to define the function
		// fit
		h_fitted[p]->Fit("fit","","",x_fit_min,x_fit_max);
		npoints = (fit->GetNumberFitPoints())-3;
		if (npoints>0) 
		    chisqr = fit->GetChisquare()/npoints;
		else
		    chisqr = 9999;
	//	const = fit->GetParameter(0);
		mean = fit->GetParameter(1);
		sigma = fit->GetParameter(2);
        
		// Results
		cout << "Fit the peak n." << p << ": with TSpectrum x = " << xPeaks[p] << endl;
		cout << "                    with the fit    x = " << mean << " and sigma = " << sigma << endl;
    
        
        // save data in txt file
            
          ofstream f;
          f.open ("fitresults.txt", std::ofstream::out | std::ofstream::app);


            f << mean << "\t" << sigma << '\n';

            f.close();
        
        
		fit->Delete();
        
	}

	// http://root.cern.ch/viewvc/trunk/tutorials/fit/multifit.C?view=markup

}



TH1F* h = (TH1F*) (h_na->Clone());

	h->GetYaxis()->SetTitle("Counts");
	h->GetXaxis()->SetTitle("Energy [keV]");
	h->SetTitle("Spectrum");
	h->GetYaxis()->SetTitleSize(0.04);
	h->GetYaxis()->SetTitleOffset(1.25);
	h->GetXaxis()->SetTitleSize(0.04);
	h->GetXaxis()->SetTitleOffset(1.25);
	h->SetLineColor(12);

	const int nBins = h->GetXaxis()->GetNbins();
	double newBins[nBins + 1];
	for(int i = 0; i <= nBins; i++) {
		newBins[i] = a + b * (h->GetBinLowEdge(i+1));
	}
	h->SetBins(nBins, newBins);





	h->Draw();
	g1->Draw("same");

	TLatex* t1 = new TLatex(1300, 50, "{}^{40}K");
	t1->SetTextSize(0.05);
	t1->Draw("same");




