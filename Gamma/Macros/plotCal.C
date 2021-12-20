#include <iostream>
#include <string>
#include "RootStyle.cc"

/*requested files: histogramOfData.root, histogramOfBackground.root, channel*/
using namespace std;

void plot(string dataFileName, short chan, double conversion_factor_BG)
{
    set_root_style(1);

    /*Reading histogram file from the Root file*/

    //data
    const string bgFileName = "histo_bkg.root";

    TFile *dataFile = new TFile(dataFileName.c_str());
    TH1F *hdata = (TH1F*)dataFile->Get(Form("ch%i",chan));
    //background
    //double conversion_factor_BG = (20)/20;
    TFile *bgFile = new TFile(bgFileName.c_str());
    TH1F *hbg = (TH1F*)bgFile->Get(Form("ch%i",chan));

    /* This will help us to find the corret renormalization of the background file since we have different duration*/
    TH1F* hbgConverted = (TH1F*)hbg->Clone("h");
    double temp;

    for (int i = 1; i<=hbg->GetXaxis()->GetNbins(); i++) 
    {
        temp = conversion_factor_BG * hbg->GetBinContent(i); 
        hbgConverted->SetBinContent(i, temp);
    }

    // set calibration function parameters
    Double_t a;
    Double_t b;

    //different calibrations according to the parameters needed for the 2 detectors


     if (chan == 0) //NaI
    {
        a = -8.97259;
        b = 0.0878097;
    }

    if (chan == 1) //HPGe
    {
        a = -0.019234;
        b = 0.148907;
    }

    //calibrating x-axis for data spectrum
    int max_binData = hdata->GetNbinsX(); 
	float max_kevData = hdata->GetBinCenter(max_binData)*b + a;
	hdata->GetXaxis()->SetLimits(a,max_kevData);




    //same for background
    int max_binBg = hbgConverted->GetNbinsX(); 
	float max_kevBg = hbgConverted->GetBinCenter(max_binBg)*b + a;
    hbgConverted->GetXaxis()->SetLimits(a,max_kevBg);

    //subtracting bin-to-bin the histogram of bg from the data one
    hdata->Add(hbgConverted, -1.);

    cout << max_kevBg << "\t" << max_kevData;
    //hdata->GetYaxis()->SetRangeUser(0, 201);
    hdata->Rebin(2);
    hdata->GetXaxis()->SetRangeUser(10, 2000);

    float w;
    w = hdata->GetXaxis()->GetBinWidth(0);
    hdata->SetMinimum(0);
    hdata->GetXaxis()->SetTitle("Energy [keV]");
    hdata->GetYaxis()->SetTitle(Form("Counts/%0.1f keV",w));
    hdata->SetStats(kFALSE);

    if(chan == 0) {
        hdata->SetTitle("Bricks spectrum - NaI(Tl) detector ");
    }
    if(chan == 1) {
        hdata->SetTitle("Bricks spectrum - HPGe detector");
    }
    hdata->GetYaxis()->SetMaxDigits(3);
    hdata->Draw();


  
    
   

    TF1 *fit1 = new TF1("g1","gaus(0)+pol1(3)", 27, 107);

    fit1->SetLineStyle(1);
	fit1->SetLineWidth(2);
    fit1->SetLineColor(kRed);
    fit1->SetParameters( 5.98198e+0,77 ,1.57943e+01, 0,0);
    //fit1->SetParLimits(2,0,20);
    hdata->Fit(fit1,"R");
    fit1->Draw("same");
    double mean = fit1->GetParameter(1);
    double sigma = fit1->GetParameter(2);
    double min = mean-3*sigma;
    double max = mean+3*sigma;


    double area =  hdata->Integral( hdata->GetXaxis()->FindBin(20),hdata->GetXaxis()->FindBin(110));
    std::cout << "area under peak 1: " << area << std::endl;
    

 

    double counts = (fit1->Integral(10,120))/hdata->GetBinWidth(0);
    std::cout << "counts under peak 1: " << counts << std::endl;

     /*

    TF1 *fit2 = new TF1("g2","gaus(0)+pol1(3)", 220, 260);

    fit2->SetLineStyle(1);
	fit2->SetLineWidth(2);
    fit2->SetLineColor(kRed);
    fit2->SetParameters(6.84451e+01,2.45986e+02 ,13, 0,0);
    //fit1->SetParLimits(2,0,20);
    //hdata->Fit(fit2,"R+");
    //fit2->Draw("same");


    double area2 =  hdata->Integral( hdata->GetXaxis()->FindBin(220),hdata->GetXaxis()->FindBin(260));
    std::cout << "area under peak 2: " <<area2<<endl;
  
    double counts2 = (fit2->Integral(220,260))/hdata->GetBinWidth(0);
    std::cout << "counts under peak 2: " << counts2 << std::endl;
   

    TF1 *fit3 = new TF1("g3","gaus(0)+pol1(3)", 275, 332);

    fit3->SetLineStyle(1);
	fit3->SetLineWidth(2);
    fit3->SetLineColor(kRed);
    fit3->SetParameters(4.84451e+01,294 ,20,0,0);
    //fit1->SetParLimits(2,0,20);
    //hdata->Fit(fit3,"R+");
    //fit3->Draw("same");


    double area3 =  hdata->Integral( hdata->GetXaxis()->FindBin(275),hdata->GetXaxis()->FindBin(330));
    std::cout << "area under peak 3: " <<area3<<endl;
  
    double counts3 = (fit3->Integral(275,330))/hdata->GetBinWidth(0);
    std::cout << "counts under peak 3: " << counts3 << std::endl;


    TF1 *fit4 = new TF1("g4","gaus(0)+pol1(3)", 331, 395);

    fit4->SetLineStyle(1);
	fit4->SetLineWidth(2);
    fit4->SetLineColor(kRed);
    fit4->SetParameters(4.84451e+01,350 ,20,0,0);
    //fit1->SetParLimits(2,0,20);
    //hdata->Fit(fit4,"R+");
    //fit4->Draw("same");


    double area4 =  hdata->Integral( hdata->GetXaxis()->FindBin(331),hdata->GetXaxis()->FindBin(395));
    std::cout << "area under peak 4: " <<area4<<endl;
  
    double counts4 = (fit4->Integral(331,395))/hdata->GetBinWidth(0);
    std::cout << "counts under peak 4: " << counts4 << std::endl;
     double mean4 = fit4->GetParameter(1);
    cout << mean4;

*/


 
}



