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
    TF1 *fit1 = new TF1("g1","gaus(0)+pol1(3)", 7500, 9200);
    TF1 *fit2 = new TF1("g2", "gaus(0)+pol1(3)", 19000, 21800);
    //TF1 *fit_a = new TF1("ga", "gaus(0)+pol1(3)", 700, 1400);
    


    fit1->SetLineStyle(1);
	fit1->SetLineWidth(3);
    fit1->SetParameters(1500,9200,500, 450.217,-0.0488353); //for D1,D2,D3
    //fit1->SetParameters(1.25784e+04,2.99596e+03,1.05122e+02, 450.217,-0.0488353); //forD4

    fit2->SetLineStyle(1);
	fit2->SetLineWidth(3);
    fit2->SetLineColor(kViolet);
	fit2->SetParameters(2.72700e+03,20000,1.96384e+02, 450.217,-0.0488353); //for D1,D2,D3
    //fit2->SetParameters(2.62637e+03,7.19168e+03,2.07706e+02, 450.217,-0.0488353); //for D4

    //fit_a->SetLineStyle(1);
	//fit_a->SetLineWidth(3);
    //fit_a->SetLineColor(kBlue);
	//fit_a->SetParameters(2.72700e+03,1000,1.96384e+02, 450.217,-0.0488353);

    h->Fit(fit1,"R");
    h->Fit(fit2,"R+");
    //h->Fit(fit_a,"R+");

    // Get parameters 
    double mean_1 = fit1->GetParameter(1);
    double mean_2 = fit2->GetParameter(1);
    //double mean_a = fit_a->GetParameter(1);

    double sigma_1 = fit1->GetParameter(2);
    double sigma_2 = fit2->GetParameter(2);
    //double sigma_a = fit_a->GetParameter(2);


    // Setting histogram info 
    h->SetMinimum(1);
    h->GetXaxis()->SetTitle("ADC counts");
    h->GetYaxis()->SetTitle("Counts");


    // Setting canvas info  and drawing histogram
    TCanvas* c1 = new TCanvas("c1","Plot of not calibrated spectra",900,900);
    h->Draw();
    fit1->Draw("SAME");
    fit2->Draw("SAME");
    //fit_a->Draw("SAME");
            

    // Coordinates for linear fit
    double_t x[2],y[2];
    //x[0] = mean_a;
    x[0] = mean_1;
    x[1] = mean_2;
    //y[0] = 59.5409;
    y[0] = 511;
    y[1] = 1274.537;
    
   

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
    double a = f1->GetParameter(0);
    double b = f1->GetParameter(1);
    TCanvas* c2 = new TCanvas("c2","Linear Fit for calibration",900,900);
    TMultiGraph* mg = new TMultiGraph();
    
    mg->Add(g); 
    mg->Draw("AP"); 
    mg->SetTitle("Detector 2");
    mg->GetXaxis()->SetTitle("Mean value (ADC count) ");
    mg->GetYaxis()->SetTitle("E (keV)");
    f1->Draw("SAME"); // add the fit line


    // Plot the calibrated spectra

    TCanvas* c3 = new TCanvas("c3","Plot of calibrated spectra",900,900);
    gPad->SetLeftMargin(0.12);

   // Re-fitting
    TF1 *fit3 = new TF1("fit3","gaus(0)+pol1(3)",460,560);
    TF1 *fit4 = new TF1("fit4","gaus(0)+pol1(3)",1200,1340);
    //TF1 *fit5 = new TF1("fit5","gaus(0)+pol1(3)",40,83);

           
    int max_bin = h1->GetNbinsX(); // This method returns the number of bins in x of the histogram
    //float max_kev = h1->GetBinCenter(max_bin)*0.0607717 -11.041;
	float max_kev = h1->GetBinCenter(max_bin)*b + a;
	h1->GetXaxis()->SetLimits(a,max_kev);

    fit3->SetLineStyle(1);
	fit3->SetLineWidth(2);
	fit3->SetParameter(1, 1173);
    fit3->SetParameters(6260,511,17, 450.217,-0.0488353);

    fit4->SetLineStyle(1);
	fit4->SetLineWidth(2);
    fit4->SetLineColor(kViolet);
    fit4->SetParameter(1, 1332);
	fit4->SetParameters(1030,1275,30, 450.217,-0.0488353);

    //fit5->SetLineStyle(1);
	//fit5->SetLineWidth(2);
    //fit5->SetLineColor(95);
    //fit5->SetParameter(1, 59);
	//fit5->SetParameters(1030,59,30, 450.217,-0.0488353);

    h1->Fit(fit3,"R");
    h1->Fit(fit4,"R+");
    //h1->Fit(fit5,"R+");


    double mean_1cal = fit3->GetParameter(1);
    double mean_2cal = fit4->GetParameter(1);
   // double mean_acal = fit5->GetParameter(1);

    double sigma_1cal = fit3->GetParameter(2);
    double sigma_2cal = fit4->GetParameter(2);
   // double sigma_acal = fit5->GetParameter(2);

    double meanerr = fit3->GetParError(1);

    /* -- Compute FWHM and resolution [%] -- */
    double fwhm1 = 2*sqrt(2*log(2))* sigma_1cal;
    double fwhm2 = 2*sqrt(2*log(2))* sigma_2cal;

    double res511 = fwhm1/mean_1cal;
    double res1275 = fwhm2/mean_2cal;
    double err = meanerr/mean_1cal;

    cout << "Resolution peak @ 511keV = " << res511 * 100 << "%" << endl;

    cout << "Resolution peak @ 1275keV = " << res1275 * 100 << "%" << endl;
    
    cout << "Error  = " << err * 100 << "%" << endl;
    
    float w = h1->GetBinWidth(0);

    h1->GetXaxis()->SetTitle("Energy [keV]");
    h1->GetYaxis()->SetTitle(Form("Counts/%0.1f keV",w));
    h1->GetXaxis()->SetRangeUser(0, 1500);
    h1->GetYaxis()->SetRangeUser(0, 58000);
    h1->GetYaxis()->SetMaxDigits(4);
   
    h1->Draw();
    fit3->Draw("SAME");
    fit4->Draw("SAME");
    //fit5->Draw("SAME");
    
     
}
