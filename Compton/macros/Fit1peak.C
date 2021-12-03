
#include <vector>
#include "RootStyle.cc"
/*  Function for linear fit */
double_t LinearFit(double_t *x, double_t *par) 
{
   double_t fitval2 = par[0] + par[1]*x[0];
  return fitval2;
}


void gaussianfit(const string infilename,short chan)
{
    set_root_style(1);
    
   //  Reading histogram file from the Root file 
    TFile *infile = new TFile(infilename.c_str());
    TH1F *h = (TH1F*)infile->Get(Form("ch%i",chan));
    TH1F* h1 = (TH1F*)h->Clone("h1");

    /* -- Fitting the two gaussians  -- */
    TF1 *fit1 = new TF1("g1","gaus(0)+pol1(3)", 8500, 10500);
    
    fit1->SetLineStyle(1);
	fit1->SetLineWidth(2);
    fit1->SetParameters(4780,9.54698e+03,3.20453e+022, 450.217,-9.31430e-02);

    h->Fit(fit1,"R");
   
    /* -- Get parameters  -- */
    double mean_1 = fit1->GetParameter(1);
    double sigma_1 = fit1->GetParameter(2);
 

    /* -- Setting histogram info  -- */    
    h->GetXaxis()->SetTitle("ADC counts");
    h->GetYaxis()->SetTitle("Counts");
    gStyle->SetOptStat(0000);


    /* -- Setting canvas info  and drawing histogram-- */    
    TCanvas* c1 = new TCanvas("c1","Plot of not calibrated spectra",1080,1020);
    
    h->Draw();
    fit1->Draw("SAME");
   

     /* -- using a and b previously obtained from calibration: YOU NEED TO CHANGE THIS VALUES  -- */

    double_t a = -7.32807;
    double_t b = 0.0544615;

    TCanvas* c3 = new TCanvas("c3","Plot of calibrated spectra",900,900);

    /*Re-fitting*/
    TF1 *fit3 = new TF1("fit3","gaus(0)+pol1(3)",465,560);
    
    int max_bin = h1->GetNbinsX(); // This method returns the number of bins in x of the histogram
	float max_kev = h1->GetBinCenter(max_bin)*b + a;
	h1->GetXaxis()->SetLimits(a,max_kev);

    
    fit3->SetLineStyle(1);
	fit3->SetLineWidth(2);
	fit3->SetParameter(1, 511);
    fit3->SetParameters(2000,511,17, 450.217,-0.0488353);

    h1->Fit(fit3,"R");
    

    double mean_1cal = fit3->GetParameter(1);
    //double mean_2cal = fit2->GetParameter(0);

    double sigma_1cal = fit3->GetParameter(2);
    // double sigma_2cal = fit2->GetParameter(1);*/

    float w = h1->GetBinWidth(0);
    h1->GetXaxis()->SetTitle("Energy [keV]");
    h1->GetYaxis()->SetTitle(Form("Counts/%0.1f keV",w));
    h1->GetXaxis()->SetRangeUser(0, 1500);
    h1->GetYaxis()->SetRangeUser(0, 10001);
    h1->GetYaxis()->SetMaxDigits(3);
    //TGaxis::SetMaxDigits(2);

    h1->Draw();
    fit3->Draw("SAME");
   
    TPaveText *pt = new TPaveText(0.1,0.1,0.2,0.2,"blNDC");
    pt->AddText("#mu = (513.00 #pm 0.04) keV");
    pt->AddText("#sigma = (16.97 #pm 0.04) keV");
    pt->SetFillColor(0);
    pt->SetTextAlign(22);
    pt->SetBorderSize(0);
    pt->Draw("same");
}
