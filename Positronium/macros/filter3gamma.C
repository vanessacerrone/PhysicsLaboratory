#include "gethisto.C"
#include <vector>


void filter(const string name_file, int numBins, double minX, double maxX) {
	// variables
	slimport_data_t indata1,indata2,indata3;
	TFile *infile = new TFile(name_file.c_str());
	TTree *intree = (TTree*)infile->Get("acq_tree_0");

	TBranch *inbranch1 = intree->GetBranch("acq_ch0");
    TBranch *inbranch2 = intree->GetBranch("acq_ch1");
    TBranch *inbranch3 = intree->GetBranch("acq_ch2");

	inbranch1->SetAddress(&indata1.timetag);
    inbranch2->SetAddress(&indata2.timetag);
    inbranch3->SetAddress(&indata2.timetag);

	TH1F *h_spectrum1 = new TH1F("h1","Detector 1",numBins,minX,maxX);
    TH1F *h_spectrum2 = new TH1F("h2","Detector 2",numBins,minX,maxX);
    TH1F *h_spectrum3 = new TH1F("h3","Detector 3",numBins,minX,maxX);
    TH1F *h_fil1 = new TH1F("h1f","Detector 1 filtered",numBins,minX,maxX);
    TH1F *h_fil2 = new TH1F("h2f","Detector 2 filtered",numBins,minX,maxX);
    TH1F *h_fil3 = new TH1F("h2f","Detector 3 filtered",numBins,minX,maxX);
    TH1F *h_sum = new TH1F("hsum","3 photons decay - Sum spectrum",numBins,minX,maxX);

	// histogram filling
    double entry1,entry2,entry3;
    
	for (int i=0; i<inbranch1->GetEntries(); i++) {
        inbranch2->GetEntry(i);
        entry2 =  -21.9585 + indata2.qlong*0.114419  ;
		inbranch1->GetEntry(i);
        entry1 = -17.8202 + indata1.qlong*0.115666;
        inbranch3->GetEntry(i);
        entry3 = -23.5959 + indata1.qlong*0.110415;
        
        h_spectrum1->Fill(entry1);
        h_spectrum2->Fill(entry2);
        h_sum->Fill(entry1+entry2+entry3);

		if ((entry1 + entry2 + entry3) < 1124.2 && (entry1 + entry2 + entry3) > 919.8 ) {
            //if (entry1 < 374 && entry1 > 307 && entry2 < 374 && entry2 > 307 && entry3 < 374 && entry3 > 307){

			h_fil1->Fill(entry1);
            h_fil2->Fill(entry2);
            h_fil3->Fill(entry2);
            
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
    h_spectrum1->GetXaxis()->SetRangeUser(0, 1200);
    h_spectrum1->GetYaxis()->SetRangeUser(0, 20000);
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
    h_spectrum2->GetXaxis()->SetRangeUser(0, 1200);
    h_spectrum2->GetYaxis()->SetRangeUser(0, 20000);
    h_spectrum2->SetLineWidth(2);
    h_spectrum2->Draw();
    

    h_fil2->SetLineColor(kRed);
    h_fil2->SetLineWidth(2);
    h_fil2->Draw("same");

    auto legend2 = new TLegend(0.67,0.75,0.85,0.87);
    legend2->AddEntry(h_spectrum2,"Raw data","l");
    legend2->AddEntry(h_fil2,"Filtered data","l");
    legend2->Draw();



    TCanvas* c3 = new TCanvas("c3","D3",1080,1020);
    gPad->SetLeftMargin(0.12);
    gStyle->SetOptStat(0000);

    h_spectrum3->SetMinimum(1);
    h_spectrum3->GetXaxis()->SetTitle("Energy [keV]");
    h_spectrum3->GetXaxis()->SetLabelOffset(0.01);
    h_spectrum3->GetXaxis()->SetLabelSize(0.035);
    h_spectrum3->GetXaxis()->SetTitleSize(0.04);
    h_spectrum3->GetXaxis()->SetTitleOffset(1.15);
    h_spectrum3->GetYaxis()->SetTitle("Counts");
    h_spectrum3->GetYaxis()->SetLabelOffset(0.008);
    h_spectrum3->GetYaxis()->SetLabelSize(0.035);
    h_spectrum3->GetYaxis()->SetTitleSize(0.04);
    h_spectrum3->GetYaxis()->SetTitleOffset(1.3);
    h_spectrum3->GetXaxis()->SetRangeUser(0, 2000);
    h_spectrum3->GetYaxis()->SetRangeUser(0, 20000);
    h_spectrum3->SetLineWidth(2);
    h_spectrum3->Draw();
    

    h_fil3->SetLineColor(kRed);
    h_fil3->SetLineWidth(2);
    h_fil3->Draw("same");

    auto legend3 = new TLegend(0.67,0.75,0.85,0.87);
    legend3->AddEntry(h_spectrum2,"Raw data","l");
    legend3->AddEntry(h_fil2,"Filtered data","l");
    legend3->Draw();

    //c1->SaveAs("D1_filtered.pdf");
    //c2->SaveAs("D2_filtered.pdf");
   
   TCanvas* c4 = new TCanvas("c4","Sum",1080,1020);
    gPad->SetLeftMargin(0.12);
    gStyle->SetOptStat(0000);

    h_sum->SetMinimum(1);
    h_sum->GetXaxis()->SetTitle("Energy [keV]");
    h_sum->GetXaxis()->SetLabelOffset(0.01);
    h_sum->GetXaxis()->SetLabelSize(0.035);
    h_sum->GetXaxis()->SetTitleSize(0.04);
    h_sum->GetXaxis()->SetTitleOffset(1.15);
    h_sum->GetYaxis()->SetTitle("Counts");
    h_sum->GetYaxis()->SetLabelOffset(0.008);
    h_sum->GetYaxis()->SetLabelSize(0.035);
    h_sum->GetYaxis()->SetTitleSize(0.04);
    h_sum->GetYaxis()->SetTitleOffset(1.3);
    h_sum->GetXaxis()->SetRangeUser(0, 1900);
    h_sum->GetYaxis()->SetRangeUser(0, 4000);
    //h_sum->SetFillColor(kAzure-9);
    h_sum->SetFillColorAlpha(kAzure-9,0.5);
    h_sum->SetLineWidth(1);
    h_sum->Draw();
    


    //c1->SaveAs("D1_filtered.pdf");
    //c2->SaveAs("D2_filtered.pdf");
    //c4->SaveAs("3gamma_sum.pdf");


    TFile *outfile = new TFile("2Photons_filter.root","RECREATE");
    outfile -> cd();
    h_spectrum1->Write();
    h_spectrum2->Write();
    h_fil1->Write();
    h_fil2->Write();

    delete outfile;
}

