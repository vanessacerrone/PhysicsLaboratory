/*-- Perform Compton events angular distribution analysis *--/

/*
 * Author      : Vanessa
 * Description : Tagger, scatterer and detector spectra ( both raw and filtered ) 
                 are saved in histograms and plotted for different scattering angles; 
                 different filtering conditions are applied 


 * Usage       : $ cd /path/to/macro
 *               $ root
 *               # .L angCorrelation.C
 *               # correlation()
*/

#include <vector>
#include <string>
#include <iostream>
#include "TBranch.h"
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "RootStyle.cc"

using namespace std;


struct slimport_data_t {

	ULong64_t	timetag;
	UInt_t		baseline;
	UShort_t	qshort;
	UShort_t	qlong;
	UShort_t	pur;
	UShort_t	samples[4096];
};



vector<string> filelist = {
    "../angular/0deg.root",
    "../angular/20deg.root",
    "../angular/40deg.root",
    "../angular/60deg.root",
    "../angular/70deg.root",
    "../angular/90deg_3.root"
    };


vector<string> outfilelist = {
    "../correlation_data/0deg.root",
    "../correlation_data/20deg.root",
    "../correlation_data/40deg.root",
    "../correlation_data/60deg.root",
    "../correlation_data/70deg.root",
    "../correlation_data/90deg.root"
    };


// compute scattered electron energy
double getEnergy(double angle) {

	double thEnergy = 511.;
	double val = 1. / (2. - TMath::Cos(angle));
	double energy = thEnergy * ( 1. - val );

	return energy;

}

void correlation()
{

  set_root_style(1);

    slimport_data_t indata_ch0,indata_ch1,indata_ch2;
    TBranch *inbranch_0;
	TBranch *inbranch_1;
    TBranch *inbranch_2;

    int N = 6;

    TTree *intree[6];
    TH1F **h0 = new TH1F*[6];  // tagger histograms
    TH1F **h1 = new TH1F*[6];  // scatterer histograms 
    TH1F **h2 = new TH1F*[6];  // detector histograms

    TH1F **h0_c = new TH1F*[6];  // tagger histograms for correlation
    TH1F **h1_c = new TH1F*[6];  // scatterer histograms for correlation
    TH1F **h2_c = new TH1F*[6];  // detector histograms for correlation

    TH2F *Correlation = new TH2F("correlation", "Correlation", 200, 0, 500, 200, 0, 500);

    TFile *infile[6];
    TFile *outfile[6];

    int angles[6] = { 0, 20, 40, 60,70, 90 };
    double conv = M_PI/180;
    double anglesrad[6] = { 0, 20*conv, 40*conv, 60*conv,70*conv, 90*conv };
    
    ULong64_t n_events_ch0[6];
    ULong64_t n_events_ch1[6];
    ULong64_t n_events_ch2[6];

    Double_t scatterer_energy;
    Double_t tagger_energy;
    Double_t detector_energy;
    Double_t sum;


    // set tolerance to 8 %
	Double_t tolerance = 0.04;
	Double_t minEnergy = (1 - tolerance) * 511.;
	Double_t maxEnergy = (1 + tolerance) * 511.;


    for(int i=0;i < N;i++){

        infile[i] = new TFile(filelist[i].c_str());
        outfile[i] = new TFile(outfilelist[i].c_str(),"RECREATE");
        intree[i] = (TTree*) infile[i]->Get("acq_tree_0");

        inbranch_0 = intree[i]->GetBranch("acq_ch0");
        inbranch_1 = intree[i]->GetBranch("acq_ch1");
        inbranch_2 = intree[i]->GetBranch("acq_ch2");

        n_events_ch1[i] = inbranch_1->GetEntries();

        inbranch_0->SetAddress(&indata_ch0.timetag);
        inbranch_1->SetAddress(&indata_ch1.timetag);
	    inbranch_2->SetAddress(&indata_ch2.timetag);

        h0[i] = new TH1F(Form("h0%d",i), Form("Tagger spectrum @ angle = %i#circ",angles[i]),1000,0,1000);
        h1[i] = new TH1F(Form("h1%d",i), Form("Scatterer spectrum @ angle = %i#circ",angles[i]),1000,0,1700);
        h2[i] = new TH1F(Form("h2%d",i), Form("Detector spectrum @ angle = %i#circ",angles[i]),1000,0,1700);

        h0_c[i] = new TH1F(Form("h0_c%d",i), Form("Cleaned Tagger spectrum @ angle = %i#circ",angles[i]),300,0,1000);
        h1_c[i] = new TH1F(Form("h1_c%d",i), Form("Cleaned Scatterer spectrum @ angle =%i#circ",angles[i]),300,0,1000);
        h2_c[i] = new TH1F(Form("h2_c%d",i), Form("Cleaned Detector spectrum @ angle = %i#circ",angles[i]),300,0,1000); 

        for(int j=0; j<n_events_ch1[i]; j++){


            inbranch_0->GetEntry(j);
            inbranch_1->GetEntry(j);
            inbranch_2->GetEntry(j);


            tagger_energy = (-7.32807 +  0.0544615*indata_ch0.qlong);
            scatterer_energy = (-7.81986 +  0.0577517*indata_ch1.qlong);
            detector_energy = (-6.78006 +  0.0531775*indata_ch2.qlong);
            sum = detector_energy + scatterer_energy;

            h0[i]->Fill(tagger_energy);
            h1[i]->Fill(scatterer_energy);
            h2[i]->Fill(detector_energy);

          if ( tagger_energy > 460 && tagger_energy < 561 ){
          if( anglesrad[i] == 0 ) {
					if( scatterer_energy < 0. + 150. ) {
						if( (sum > minEnergy) && (sum < maxEnergy) ) {
							h0_c[i]->Fill(tagger_energy);
                            h1_c[i]->Fill(scatterer_energy);
                            h2_c[i]->Fill(detector_energy);
						}
					}
				} else {

          //if( scatterer_energy > 0.6*getEnergy(anglesrad[i]) && scatterer_energy < 1.2*getEnergy(anglesrad[i])) {

			if( sum > minEnergy && sum < maxEnergy )
				{
					h0_c[i]->Fill(tagger_energy);
                    h1_c[i]->Fill(scatterer_energy);
                    h2_c[i]->Fill(detector_energy);

                    if (i==5){
                        
                    Correlation->Fill(scatterer_energy,detector_energy);
      
                        }
				    }
					
			    }
            }

          //}

        }

        //h0_c[i]->Write();
        //h1_c[i]->Write();
        //h2_c[i]->Write();

        //outfile[i]->Close();
    }
	




    TCanvas* c0 = new TCanvas("c0", "canvas", 550, 1200);
        c0->Divide(2, 3, 0.0005, 0.0005);
        float w0;
        for (int j=0; j < N; j++) {
        w0 = h0_c[j]->GetBinWidth(0);
        c0->cd(j+1);
        h0_c[j]->GetXaxis()->SetTitle("Energy [keV]");
        h0_c[j]->GetYaxis()->SetTitle(Form("Counts/%0.1f keV",w0));
        h0_c[j]->GetYaxis()->SetMaxDigits(3);
        h0_c[j]->Draw("same");

        }
    


    TCanvas* c1 = new TCanvas("c1", "canvas", 550, 1200);
        c1->Divide(2, 3, 0.0005, 0.0005);
        float w1;
        for (int j=0; j < N; j++) {
        w1 = h1_c[j]->GetBinWidth(0);
        c1->cd(j+1);
        h1_c[j]->GetXaxis()->SetTitle("Energy [keV]");
        h1_c[j]->GetYaxis()->SetTitle(Form("Counts/%0.1f keV",w1));
        h1_c[j]->GetYaxis()->SetMaxDigits(3);
        h1_c[j]->Draw("same");

        }
    
        TCanvas* c2 = new TCanvas("c2", "canvas", 550, 1200);
        c2->Divide(2, 3, 0.0005, 0.0005);
        float w2;
        for (int j=0; j < N; j++) {
        w2 = h2_c[j]->GetBinWidth(0);
        c2->cd(j+1);
        h2_c[j]->GetXaxis()->SetTitle("Energy [keV]");
        h2_c[j]->GetYaxis()->SetTitle(Form("Counts/%0.1f keV",w2));
        h2_c[j]->GetYaxis()->SetMaxDigits(3);
        h2_c[j]->Draw("same");

        }

    TCanvas* c3 = new TCanvas("c3", "canvas", 800, 800);
    TF1 *f = new TF1("f", "[0] + [1] * x", 0, 700 );
	f->SetParameters(511., -1.);
	f->SetLineColor(kRed);
    Correlation->SetTitle("2D correlation plot at angle = 90#circ");
    Correlation->GetXaxis()->SetTitle("Scatterer Energy [keV]");
    Correlation->GetYaxis()->SetTitle("Detector Energy [keV]");
	Correlation->Fit("f");
    Correlation->Draw("colz1");
    f->Draw("same");

    
    TPaveText *pt = new TPaveText(0.1,0.1,0.2,0.2,"blNDC");
    pt->AddText("Fit function y = a + bx");
    pt->AddText("a = (504 #pm 2) keV");
    pt->AddText("b = (-0.97 #pm 0.01)");
    pt->SetFillColor(0);
    pt->SetTextAlign(22);
    pt->SetBorderSize(0);
    pt->Draw("same");

   
  


    TCanvas* c4 = new TCanvas("c4", "canvas", 800, 800);
    //TColor::InvertPalette();
    Correlation->SetTitle("2D correlation plot at angle = 90#circ");
    Correlation->GetXaxis()->SetTitle("Scatterer Energy [keV]");
    Correlation->GetYaxis()->SetTitle("Detector Energy [keV]");
    //Correlation->GetYaxis()->SetTitleOffset(1.7);
    //Correlation->GetXaxis()->SetTitleOffset(1.7);
    Correlation->Draw("lego2z");



  
}
