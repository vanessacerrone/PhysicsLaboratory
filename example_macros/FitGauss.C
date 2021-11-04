#include "gethisto.C"
#include <vector>

            /*  Function for linear fit */
double_t LinearFit(double_t *x, double_t *par) 
{
   double_t fitval2 = par[0] + par[1]*x[0];
  return fitval2;
}


void Analysis(const string file_na,short chan)
{
        /*  Reading histogram file from the Tree  */
    TH1F* h = getHistoForChannelFromTree(file_na.c_str(), chan,1026,0,16384);
    TH1F* h1 = (TH1F*)h->Clone("h1");

                /* -- Fitting the two gaussians  -- */
    TF1 *fit1 = new TF1("fit1","gaus",4450,5000);
    TF1 *fit2 = new TF1("fit2","gaus",10750,12000);
    //TF1* total= new TF1("fitTotal","gaus(0=+gaus(3)",4450,12000);

    h->Fit(fit1,"R");
    h->Fit(fit2,"R+");

                    /* -- Get parameters  -- */
    double mean_1 = fit1->GetParameter(1);
    double mean_2 = fit2->GetParameter(1);

    double sigma_1 = fit1->GetParameter(2);
    double sigma_2 = fit2->GetParameter(2);

                  /* -- Setting histogram info  -- */    
    h->SetMinimum(1);
    h->GetXaxis()->SetTitle("ADC counts");
    h->GetXaxis()->SetLabelOffset(0.01);
    h->GetXaxis()->SetLabelSize(0.04);
    h->GetXaxis()->SetTitleSize(0.04);
    h->GetXaxis()->SetTitleOffset(1.15);
    h->GetYaxis()->SetTitle("Entries");
    h->GetYaxis()->SetLabelOffset(0.008);
    h->GetYaxis()->SetLabelSize(0.04);
    h->GetYaxis()->SetTitleSize(0.04);
    h->GetYaxis()->SetTitleOffset(1.25);
    gStyle->SetOptStat(1111);


            /* -- Setting canvas info  and drawing histogram-- */    
    TCanvas* c1 = new TCanvas("c1","Plot of not calibrated spectra",1080,1020);
    h->Draw();

            /* -- Now we need to compute the calibration function  -- */    


                         /* -- double_t for coordinates  -- */    
    double_t x[2],y[2];
    x[0]=mean_1;
    x[1]=mean_2;
    y[0]=511;
    y[1]=1275;
   

                        /* -- Setting fit info and drawing options-- */    
    TF1 *f1 = new TF1("f1", LinearFit, 0, 30000, 2);
    TGraph *g = new TGraph(2,x,y);
    g->SetMarkerStyle(8);
    g->SetMarkerSize(1.2);
    g->SetMarkerColor(1);
    f1->SetLineStyle(2);
    f1->SetLineColor(2);
    f1->SetLineWidth(2);
    f1->SetParameters(0,0);
    f1->SetParNames("a", "b");

            /*Save fit values and prepare canvas*/
    g->Fit(f1,"R");
    double a= f1->GetParameter(0);
    double b = f1->GetParameter(1);
    TCanvas* c2 = new TCanvas("c2","Linear Fit for calibration",1080,1020);
    TMultiGraph* mg = new TMultiGraph();
    
    mg->Add(g); //the graph
    mg->Draw("AP"); //drawing points
    mg->SetTitle("Calibration Detector 2");
    mg->GetXaxis()->SetTitle("Mean value (ADC count) ");
    mg->GetYaxis()->SetTitle("E (keV)");
    f1->Draw("sames"); //add the fit line

                   /* --  We can now plot the calibrated spectra, using the fit parameters obtained! -- */

    TCanvas* c3 = new TCanvas("c3","Plot of calibrated spectra",1080,1020);
        /*Re-fitting*/
    TF1 *fit3 = new TF1("fit3","gaus",510,600);
    TF1 *fit4 = new TF1("fit4","gaus",1220,1380);
    h1->GetXaxis()->Set(16384,0,a+b*16384*h->GetXaxis()->GetBinWidth(0));
     
    h1->Fit(fit3,"R");
    h1->Fit(fit4,"R+");

    double mean_1cal = fit1->GetParameter(1);
    double mean_2cal = fit2->GetParameter(1);

    double sigma_1cal = fit1->GetParameter(2);
    double sigma_2cal = fit2->GetParameter(2);

    std::cout<<"Print mean calibrated:"<<mean_1cal<<" "<<mean_2cal<<std::endl;
    h1->GetXaxis()->SetTitle("Energy [keV]");
    h1->GetXaxis()->SetLabelOffset(0.01);
    h1->GetXaxis()->SetLabelSize(0.04);
    h1->GetXaxis()->SetTitleSize(0.04);
    h1->GetXaxis()->SetTitleOffset(1.15);
    h1->GetYaxis()->SetTitle("Entries");
    h1->GetYaxis()->SetLabelOffset(0.008);
    h1->GetYaxis()->SetLabelSize(0.04);
    h1->GetYaxis()->SetTitleSize(0.04);
    h1->GetYaxis()->SetTitleOffset(1.25);
    h1->GetXaxis()->SetRangeUser(0, 1600);

    h1->Draw();

}
