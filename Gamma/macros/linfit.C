#include <iostream>
#include <string>
#include "RootStyle.cc"

double_t LinearFit(double_t *x, double_t *par) 
{
   double_t fitval2 = par[0] + par[1]*x[0];
  return fitval2;
}


void fit(const string filename) {
    set_root_style(1);
    TCanvas *c = new TCanvas("c", "c", 800, 600);

    TPad* upperPad = new TPad("lowerPad", "lowerPad",0,0.3,1,1);
    TPad* lowerPad = new TPad("upperPad", "upperPad", 0,0,1,0.3);
    upperPad->SetBottomMargin(0);
    lowerPad->SetTopMargin(0);
    upperPad->Draw(); 			       
    lowerPad->Draw(); 

    upperPad->cd();
	TGraphErrors *gr = new TGraphErrors(filename.c_str());

    gr->GetXaxis()->SetTitle("Radium sample counts (N)");
    gr->GetYaxis()->SetTitle("Exposed canister counts (E)");
    gr->GetXaxis()->SetRangeUser(0,2200);
    gr->GetYaxis()->SetRangeUser(-10, 600);
    gr->SetTitle("");
    gr->SetMarkerColor(kBlack);
    gr->GetYaxis()->SetMaxDigits(4);
    TF1 *f1 = new TF1("f1",LinearFit,0,2200,2);
    //f1->SetParameter(1,0.0881642);
    TFitResultPtr fit_result = gr->Fit(f1,"RS");
    
    
    f1->SetLineWidth(1);
    f1->SetLineColor(kRed);
    gr->Draw("ap");
    f1->Draw("same");
    fit_result->Print("V");

	// Get the parameters
	double m = f1->GetParameter(1);
    double em = f1->GetParError(1);
    double eq = f1->GetParError(1);
	double q = f1->GetParameter(0);
   
	cout << "Slope = " << m << ", Intercept = " << q << endl;


    TPaveText *pt = new TPaveText(0.1,0.1,0.2,0.2,"blNDC");
    pt->AddText("m = 0.26 #pm 0.01 ");
    pt->AddText("q = 20 #pm 9");
    pt->AddText("#chi^{(2)}/ ndof = 1.1/4");
    pt->SetFillColor(0);
    pt->SetBorderSize(0);
    pt->Draw("same");


    TPaveText *pt1 = new TPaveText(0.1,0.1,0.2,0.2,"blNDC");
    pt1->SetFillColor(0);
    pt1->AddText("609 keV ");
    pt1->Draw("same");

    TPaveText *pt2 = new TPaveText(0.1,0.1,0.2,0.2,"blNDC");
    pt2->SetFillColor(0);
    pt2->AddText("1120 keV ");
    pt2->Draw("same");

    TPaveText *pt3 = new TPaveText(0.1,0.1,0.2,0.2,"blNDC");
    pt3->SetFillColor(0);
    pt3->AddText("242 keV ");
    pt3->Draw("same");

    TPaveText *pt4 = new TPaveText(0.1,0.1,0.2,0.2,"blNDC");
    pt4->SetFillColor(0);
    pt4->AddText("295 keV ");
    pt4->Draw("same");

    TPaveText *pt5 = new TPaveText(0.1,0.1,0.2,0.2,"blNDC");
    pt5->SetFillColor(0);
    pt5->AddText("352 keV ");
    pt5->Draw("same");


    TPaveText *pt6 = new TPaveText(0.1,0.1,0.2,0.2,"blNDC");
    pt6->SetFillColor(0);
    pt6->AddText("1765 keV ");
    pt6->Draw("same");



    // calculates residuals
    TGraphErrors *gr2 = new TGraphErrors(filename.c_str());
    for (int i=0; i<gr->GetN(); i++) {
      double res = gr->GetY()[i] - f1->Eval(gr->GetX()[i]); // residual
      gr2->SetPoint(i,gr->GetX()[i],res);
      double eresy = gr->GetEY()[i];
      gr2->SetPointError(i,0,eresy);
    }
    
    lowerPad->cd();
    
    gr2->SetTitle("");
    gr2->SetMarkerColor(kBlack);
    gr2->SetMarkerStyle(8);
    gr2->SetMarkerSize(0.7);
    gr2->SetLineWidth(1);
    gr2->GetXaxis()->SetTitle("Radio sample counts N");
    gr2->GetYaxis()->SetTitle("Residuals");
    gr2->Draw("ap");
    //gr2->GetYaxis()->SetRangeUser(-0.8,0.8);
    gr2->GetXaxis()->SetRangeUser(0,2200); 
    // linea di zero per i residui
    TLine *line = new TLine(0, 0, 2200, 0);
    line->SetLineStyle(2);
    line->SetLineColor(kBlack);
    line->Draw("same");
    


    

}

/*
Chi2                      =      3.54563
NDf                       =            5

p0                        =      7.70011   +/-   5.95817     
p1                        =     0.274427   +/-   0.00997598  


Chi2                      =       1.1228
NDf                       =            4
Edm                       =  3.07768e-09
NCalls                    =           51
p0                        =      20.1224   +/-   8.87217     
p1                        =     0.260419   +/-   0.0113149   

*/


//1377.669(12)
//1764.494(14)
//1120.287(10)
//241.997(3)
//295.224(2)
//609.312(7)
//351.932(2)