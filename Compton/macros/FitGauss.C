#include <iostream>
#include <string>
#include "RootStyle.cc"

/*  Function for linear fit */
double_t LinearFit(double_t *x, double_t *par) 
{
   double_t fitval2 = par[0] + par[1]*x[0];
  return fitval2;
}


void FitGauss(const string infilename,short chan)
{
    set_root_style(1);

    //  Reading histogram file from the Root file 
    TFile *infile = new TFile(infilename.c_str());
    TH1F *h = (TH1F*)infile->Get(Form("ch%i",chan));

    TH1F* h1 = (TH1F*)h->Clone("h1");

    // Fitting the two gaussians 
    TF1 *fit1 = new TF1("g1","gaus(0)+pol1(3)", 3900, 5300);
    TF1 *fit2 = new TF1("g2", "gaus(0)+pol1(3)", 10450, 12000);
    


    fit1->SetLineStyle(1);
	fit1->SetLineWidth(3);
	fit1->SetParameter(1, 4500);
    fit1->SetParameters(1.25804e+04,4500+03,1.05104e+02, 450.217,-0.0488353); //for D1,D2,D3
    //fit1->SetParameters(1.25784e+04,2.99596e+03,1.05122e+02, 450.217,-0.0488353); //forD4

    fit2->SetLineStyle(1);
	fit2->SetLineWidth(3);
    fit2->SetLineColor(kTeal+2);
	fit2->SetParameters(2.72700e+03,11000,1.96384e+02, 450.217,-0.0488353); //for D1,D2,D3
    //fit2->SetParameters(2.62637e+03,7.19168e+03,2.07706e+02, 450.217,-0.0488353); //for D4

    h->Fit(fit1,"R");
    h->Fit(fit2,"R+");

    // Get parameters 
    double mean_1 = fit1->GetParameter(1);
    double mean_2 = fit2->GetParameter(1);

    double sigma_1 = fit1->GetParameter(2);
    double sigma_2 = fit2->GetParameter(2);

    // Setting histogram info 
    h->SetMinimum(1);
    h->GetXaxis()->SetTitle("ADC counts");
    h->GetYaxis()->SetTitle("Counts");


    // Setting canvas info  and drawing histogram
    TCanvas* c1 = new TCanvas("c1","Plot of not calibrated spectra",1080,1020);
    h->Draw();
    fit1->Draw("SAME");
    fit2->Draw("SAME");

            

    // Coordinates for linear fit
    double_t x[2],y[2];
    x[0] = mean_1;
    x[1] = mean_2;
    y[0] = 511;
    y[1] = 1275;
   

    // Setting fit info and drawing options
    TF1 *f1 = new TF1("f1", LinearFit, 0, 30000, 2);
    TGraph *g = new TGraph(2,x,y);
    g->SetMarkerStyle(8);
    g->SetMarkerSize(1.2);
    g->SetMarkerColor(1);
    f1->SetLineStyle(1);
    f1->SetLineColor(2);
    f1->SetLineWidth(2);
    f1->SetParameters(0,0);
    f1->SetParNames("a", "b");

    g->Fit(f1,"R");
    double a= f1->GetParameter(0);
    double b = f1->GetParameter(1);
    TCanvas* c2 = new TCanvas("c2","Linear Fit for calibration",1080,1020);
    TMultiGraph* mg = new TMultiGraph();
    
    mg->Add(g); 
    mg->Draw("AP"); 
    mg->SetTitle("Detector 2");
    mg->GetXaxis()->SetTitle("Mean value (ADC count) ");
    mg->GetYaxis()->SetTitle("E (keV)");
    f1->Draw("SAME"); // add the fit line


    // Plot the calibrated spectra

    TCanvas* c3 = new TCanvas("c3","Plot of calibrated spectra",1080,1020);
    gPad->SetLeftMargin(0.12);

   // Re-fitting
    TF1 *fit3 = new TF1("fit3","gaus(0)+pol1(3)",450,600);
    TF1 *fit4 = new TF1("fit4","gaus(0)+pol1(3)",1200,1410);



    int max_bin = h1->GetNbinsX(); // This method returns the number of bins in x of the histogram
	float max_kev = h1->GetBinCenter(max_bin)*b + a;
	h1->GetXaxis()->SetLimits(a,max_kev);

    fit3->SetLineStyle(1);
	fit3->SetLineWidth(1);
	fit3->SetParameter(1, 511);
    fit3->SetParameters(6260,511,17, 450.217,-0.0488353);

    fit4->SetLineStyle(1);
	fit4->SetLineWidth(1);
    fit4->SetLineColor(kTeal+2);
    fit4->SetParameter(1, 1275);
	fit4->SetParameters(1030,1300,30, 450.217,-0.0488353);
    h1->Fit(fit3,"R");
    h1->Fit(fit4,"R+");

    double mean_1cal = fit3->GetParameter(1);
    double mean_2cal = fit4->GetParameter(1);

    double sigma_1cal = fit3->GetParameter(2);
    double sigma_2cal = fit4->GetParameter(2);

    /* -- Compute FWHM and resolution [%] -- */
    double fwhm1 = 2*sqrt(2*log(2))* sigma_1cal;
    double fwhm2 = 2*sqrt(2*log(2))* sigma_2cal;

    double res511 = fwhm1/mean_1cal;
    double res1275 = fwhm2/mean_2cal;

    cout << "Resolution peak @ 511keV = " << res511 * 100 << "%" << endl;

    cout << "Resolution peak @ 1275keV = " << res1275 * 100 << "%" << endl;
    
    
    h1->GetXaxis()->SetTitle("Energy [keV]");
    h1->GetYaxis()->SetTitle("Counts");
    h1->GetXaxis()->SetRangeUser(0, 1600);
    h1->GetYaxis()->SetRangeUser(0, 6000);
   
    h1->Draw();
    fit3->Draw("SAME");
    fit4->Draw("SAME");

   
     
}
