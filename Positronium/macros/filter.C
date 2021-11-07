#include "gethisto.C"
#include <vector>

/*
/ -- Build the sum spectra for energy for the 2 photons decay --
/
/ Raw data (already calibrated in energy) is stored in two 
/ histograms, then a filtering procedure is performed:
/ the events whose sum of the energies measured by detectors 1 
/ and 2 differs from 1022keV for more than 10% are discarded.
/ Both filtered and unfiltered histograms are saved in a .root file. 
*/ 



void filter(const string name_file, int numBins, double minX, double maxX) {
	// variables
	slimport_data_t indata1,indata2;
	TFile *infile = new TFile(name_file.c_str());
	TTree *intree = (TTree*)infile->Get("acq_tree_0");

	TBranch *inbranch1 = intree->GetBranch("acq_ch0");
    TBranch *inbranch2 = intree->GetBranch("acq_ch1");

	inbranch1->SetAddress(&indata1.timetag);
    inbranch2->SetAddress(&indata2.timetag);

	TH1F *h_spectrum1 = new TH1F("h1","Detector 1",numBins,minX,maxX);
    TH1F *h_spectrum2 = new TH1F("h2","Detector 2",numBins,minX,maxX);
    TH1F *h_fil1 = new TH1F("h1f","Detector 1 filtered",numBins,minX,maxX);
    TH1F *h_fil2 = new TH1F("h2f","Detector 2 filtered",numBins,minX,maxX);

	// histogram filling
    double entry1,entry2;
    
	for (int i=0; i<inbranch1->GetEntries(); i++) {
        inbranch2->GetEntry(i);
        entry2 =  -21.9585 + indata2.qlong*0.114419  ;
		inbranch1->GetEntry(i);
        entry1 = -17.8202 + indata1.qlong*0.115666;
        h_spectrum1->Fill(entry1);
        h_spectrum2->Fill(entry2);

		if ((entry1 + entry2) < 1124.2 && (entry1 + entry2) > 919.8) {

			h_fil1->Fill(entry1);
            h_fil2->Fill(entry2);
            
		}

	}


    TCanvas* c1 = new TCanvas("c1","D1",1080,1020);
    gPad->SetLeftMargin(0.12);
    gStyle->SetOptStat(0000);
    gStyle->SetTitleY(0.975);
    h_spectrum1->SetMinimum(1);
    h_spectrum1->GetXaxis()->SetTitle("Energy [keV]");
    h_spectrum1->GetXaxis()->SetLabelOffset(0.01);
    h_spectrum1->GetXaxis()->SetLabelSize(0.035);
    h_spectrum1->GetXaxis()->SetTitleSize(0.04);
    h_spectrum1->GetXaxis()->SetTitleOffset(1.15);
    h_spectrum1->GetYaxis()->SetTitle("Counts");
    h_spectrum1->GetYaxis()->SetLabelOffset(0.008);
    h_spectrum1->GetYaxis()->SetLabelSize(0.035);
    h_spectrum1->GetYaxis()->SetTitleSize(0.04);
    h_spectrum1->GetYaxis()->SetTitleOffset(1.3);
    h_spectrum1->GetXaxis()->SetRangeUser(200, 800);
    h_spectrum1->GetYaxis()->SetRangeUser(0, 3700);
    h_spectrum1->SetLineWidth(2);
    h_spectrum1->Draw();

    h_fil1->SetLineColor(kRed);
    h_fil1->SetLineWidth(2);
    h_fil1->Draw("same");
    
    auto legend = new TLegend(0.67,0.75,0.85,0.87);
    //TLegend *legend = new TLegend(0.16, 0.63, 0.45, 0.91);
    legend->AddEntry(h_spectrum1,"Raw data","l");
    legend->AddEntry(h_fil1,"Filtered data","l");
    legend->Draw();



    TCanvas* c2 = new TCanvas("c2","D2",1080,1020);
    gPad->SetLeftMargin(0.12);
    gStyle->SetOptStat(0000);

    h_spectrum2->SetMinimum(1);
    h_spectrum2->GetXaxis()->SetTitle("Energy [keV]");
    h_spectrum2->GetXaxis()->SetLabelOffset(0.01);
    h_spectrum2->GetXaxis()->SetLabelSize(0.035);
    h_spectrum2->GetXaxis()->SetTitleSize(0.04);
    h_spectrum2->GetXaxis()->SetTitleOffset(1.15);
    h_spectrum2->GetYaxis()->SetTitle("Counts");
    h_spectrum2->GetYaxis()->SetLabelOffset(0.008);
    h_spectrum2->GetYaxis()->SetLabelSize(0.035);
    h_spectrum2->GetYaxis()->SetTitleSize(0.04);
    h_spectrum2->GetYaxis()->SetTitleOffset(1.3);
    h_spectrum2->GetXaxis()->SetRangeUser(200, 800);
    h_spectrum2->GetYaxis()->SetRangeUser(0, 3500);
    h_spectrum2->SetLineWidth(2);
    h_spectrum2->Draw();
    

    h_fil2->SetLineColor(kRed);
    h_fil2->SetLineWidth(2);
    h_fil2->Draw("same");

    auto legend2 = new TLegend(0.67,0.75,0.85,0.87);
    legend2->AddEntry(h_spectrum2,"Raw data","l");
    legend2->AddEntry(h_fil2,"Filtered data","l");
    legend2->Draw();

    //c1->SaveAs("D1_filtered.pdf");
    //c2->SaveAs("D2_filtered.pdf");
   
    TFile *outfile = new TFile("2Photons_filter.root","RECREATE");
    outfile -> cd();
    h_spectrum1->Write();
    h_spectrum2->Write();
    h_fil1->Write();
    h_fil2->Write();

    delete outfile;
}

