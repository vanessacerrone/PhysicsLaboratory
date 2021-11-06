#include "gethisto.C"
#include <vector>

/*  Function for linear fit */
double_t LinearFit(double_t *x, double_t *par) 
{
   double_t fitval2 = par[0] + par[1]*x[0];
  return fitval2;
}


void gaussianfit(const string file_na,short chan)
{
    /* -- Reading histogram file from the Tree -- */
    TH1F* h = getHistoForChannelFromTree(file_na.c_str(),chan,1026,0,16384);
    TH1F* h1 = (TH1F*)h->Clone("h1");

    /* -- Fitting the two gaussians  -- */
    TF1 *fit1 = new TF1("g1","gaus(0)+pol1(3)", 4200, 5300);
    
    fit1->SetLineStyle(1);
	fit1->SetLineWidth(2);
    fit1->SetParameters(4780,4761,150, 450.217,-9.31430e-02);

    h->Fit(fit1,"R");
   
    /* -- Get parameters  -- */
    double mean_1 = fit1->GetParameter(1);
    double sigma_1 = fit1->GetParameter(2);
 

    /* -- Setting histogram info  -- */    
    h->SetMinimum(1);
    h->GetXaxis()->SetTitle("ADC counts");
    h->GetXaxis()->SetLabelOffset(0.01);
    h->GetXaxis()->SetLabelSize(0.04);
    h->GetXaxis()->SetTitleSize(0.04);
    h->GetXaxis()->SetTitleOffset(1.4);
    h->GetYaxis()->SetTitle("Counts");
    h->GetYaxis()->SetLabelOffset(0.008);
    h->GetYaxis()->SetLabelSize(0.04);
    h->GetYaxis()->SetTitleSize(0.04);
    h->GetYaxis()->SetTitleOffset(1);
    gStyle->SetOptStat(1111);


    /* -- Setting canvas info  and drawing histogram-- */    
    TCanvas* c1 = new TCanvas("c1","Plot of not calibrated spectra",1080,1020);
    h->Draw();
    fit1->Draw("SAME");
   

     /* -- using a and b previously obtained from calibraion  -- */

    double_t a = -17.8202;
    double_t b =  0.115666;

    TCanvas* c3 = new TCanvas("c3","Plot of calibrated spectra",1080,1020);
    /*Re-fitting*/
    TF1 *fit3 = new TF1("fit3","gaus(0)+pol1(3)",470,600);
    

    h1->GetXaxis()->Set(16384,0,a+b*16384*h->GetXaxis()->GetBinWidth(0));
    
    fit3->SetLineStyle(1);
	fit3->SetLineWidth(2);
	fit3->SetParameter(1, 511);
    fit3->SetParameters(2000,532,17, 450.217,-0.0488353);

    h1->Fit(fit3,"R");
    

    /*double mean_1cal = fit1->GetParameter(0);
    double mean_2cal = fit2->GetParameter(0);

    double sigma_1cal = fit1->GetParameter(1);
    double sigma_2cal = fit2->GetParameter(1);*/

    h1->GetXaxis()->SetTitle("Energy [keV]");
    h1->GetXaxis()->SetLabelOffset(0.01);
    h1->GetXaxis()->SetLabelSize(0.04);
    h1->GetXaxis()->SetTitleSize(0.04);
    h1->GetXaxis()->SetTitleOffset(1.4);
    h1->GetYaxis()->SetTitle("Counts");
    h1->GetYaxis()->SetLabelOffset(0.008);
    h1->GetYaxis()->SetLabelSize(0.04);
    h1->GetYaxis()->SetTitleSize(0.04);
    h1->GetYaxis()->SetTitleOffset(1);
    h1->GetXaxis()->SetRangeUser(0, 1600);
    //TGaxis::SetMaxDigits(2);

    h1->Draw();
    fit3->Draw("SAME");
   

}
