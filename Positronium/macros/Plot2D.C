#include "gethisto.C"
#include <vector>
#include <fstream>

void plot(const string name_file) {
	// variables
	slimport_data_t indata1,indata2,indata3,indata4;
	TFile *infile = new TFile(name_file.c_str());
	TTree *intree = (TTree*)infile->Get("acq_tree_0");

	TBranch *inbranch1 = intree->GetBranch("acq_ch0");
    TBranch *inbranch2 = intree->GetBranch("acq_ch1");
    TBranch *inbranch3 = intree->GetBranch("acq_ch2");
    TBranch *inbranch4 = intree->GetBranch("acq_ch3");

	inbranch1->SetAddress(&indata1.timetag);
    inbranch2->SetAddress(&indata2.timetag);
    inbranch3->SetAddress(&indata3.timetag);
    inbranch4->SetAddress(&indata4.timetag);

    TH2* h1 = new TH2F("h1", "Detector 1",400,0,4000,400,0,1400);
    TH2* h2 = new TH2F("h2", "Detector 2",400,0,4000,400,0,1400);
    TH2* h3 = new TH2F("h3", "Detector 3",400,0,4000,400,0,1400);

    TH2* h_fil = new TH2F("h3", "Filtered spectrum",300,0,4000,400,0,1400);


        

	// histogram filling
    double entry1,entry2,entry3,entry4;
    
	for (int i=0; i<inbranch1->GetEntries(); i++) {
        inbranch2->GetEntry(i);
        entry2 =  -21.7267 + indata2.qlong*0.114383  ;
		inbranch1->GetEntry(i);
        entry1 = -17.7045 + indata1.qlong*0.115652;
        inbranch3->GetEntry(i);
        entry3 = -22.9221 + indata3.qlong*0.110317;
        inbranch4->GetEntry(i);
        entry4 = -0.0201002 + 0.0116122*indata4.qlong;

        double sum = entry1+entry2+entry3;

        h1->Fill(sum,entry1);
        h2->Fill(sum,entry2);
        h3->Fill(sum,entry3);


       
		if ((entry1 + entry2 + entry3) < 1124.2 && (entry1 + entry2 + entry3) > 919.8) {
        if ( (entry1 < 450 && entry1 > 0) && (entry2 < 450 && entry2 > 0) && (entry3 < 450 && entry3 > 0)){

		h_fil->Fill(sum,entry2);
          }
        }
    }

   //gStyle->SetPalette(58);
    //gStyle->SetPalette(kColorPrintableOnGrey);
    //gStyle->SetNumberContours(70);
    gStyle->SetPalette(kBird);
        //TColor::InvertPalette();
    //TCanvas* c = new TCanvas("c", "canvas", 800, 800);
    //c->Divide(2, 2, 0.0005, 0.0005);
    //c->cd(1);
    TCanvas* c1 = new TCanvas("c1","D1",1080,1020);
    gPad->SetLeftMargin(0.12);
    gStyle->SetOptStat(0000);
    gStyle->SetTitleY(0.975);
    h1->SetMinimum(0);
    h1->SetMaximum(30);
    h1->GetXaxis()->SetTitle("E_{1} + E_{2} + E_{3} [keV]");
    h1->GetXaxis()->SetLabelOffset(0.01);
    h1->GetXaxis()->SetLabelSize(0.035);
    h1->GetXaxis()->SetTitleSize(0.04);
    h1->GetXaxis()->SetTitleOffset(1.15);
    h1->GetYaxis()->SetTitle("E_{1} [keV]");
    h1->GetYaxis()->SetLabelOffset(0.008);
    h1->GetYaxis()->SetLabelSize(0.035);
    h1->GetYaxis()->SetTitleSize(0.04);
    h1->GetYaxis()->SetTitleOffset(1.2);
    h1->GetXaxis()->SetRangeUser(0, 3000);
    h1->GetYaxis()->SetRangeUser(0, 1400);
    h1->SetLineWidth(1);
    h1->Draw("COLZ2");
    TLine *l1 = new TLine(1022,0,1022,1400);
    TLine *l2 = new TLine(0,304,3000,304);
    l1->Draw("same");
    l2->Draw("same");
   c1->Update();

    TCanvas* c2 = new TCanvas("c2","D2",1080,1020);
    //c->cd(2);

    gPad->SetLeftMargin(0.12);
    h2->SetMinimum(0);
    h2->SetMaximum(30);
    h2->GetXaxis()->SetTitle("E_{1} + E_{2} + E_{3} [keV]");
    h2->GetXaxis()->SetLabelOffset(0.01);
    h2->GetXaxis()->SetLabelSize(0.035);
    h2->GetXaxis()->SetTitleSize(0.04);
    h2->GetXaxis()->SetTitleOffset(1.15);
    h2->GetYaxis()->SetTitle("E_{2} [keV]");
    h2->GetYaxis()->SetLabelOffset(0.008);
    h2->GetYaxis()->SetLabelSize(0.035);
    h2->GetYaxis()->SetTitleSize(0.04);
    h2->GetYaxis()->SetTitleOffset(1.2);
    h2->GetXaxis()->SetRangeUser(0, 3000);
    h2->GetYaxis()->SetRangeUser(0, 1400);
    h2->SetLineWidth(1);
    h2->Draw("COLZ2");
    l1->Draw("same");
    l2->Draw("same");




    TCanvas* c3 = new TCanvas("c3","D3",1080,1020);
    //c->cd(3);
    gPad->SetLeftMargin(0.12);
    h3->SetMinimum(0);
    h3->SetMaximum(30);
    h3->GetXaxis()->SetTitle("E_{1} + E_{2} + E_{3} [keV]");
    h3->GetXaxis()->SetLabelOffset(0.01);
    h3->GetXaxis()->SetLabelSize(0.035);
    h3->GetXaxis()->SetTitleSize(0.04);
    h3->GetXaxis()->SetTitleOffset(1.15);
    h3->GetYaxis()->SetTitle("E_{3} [keV]");
    h3->GetYaxis()->SetLabelOffset(0.008);
    h3->GetYaxis()->SetLabelSize(0.035);
    h3->GetYaxis()->SetTitleSize(0.04);
    h3->GetYaxis()->SetTitleOffset(1.2);
    h3->GetXaxis()->SetRangeUser(0, 3000);
    h3->GetYaxis()->SetRangeUser(0, 1400);
    h3->SetLineWidth(1);
    h3->Draw("COLZ2");
    l1->Draw("same");
    l2->Draw("same");
    
    TCanvas* c4 = new TCanvas("c4","fil",1080,1020);
    //c->cd(4);
    gPad->SetLeftMargin(0.12);
    //h_fil->SetMinimum(0);
    h_fil->SetMaximum(25);
    h_fil->GetXaxis()->SetTitle("E_{1} + E_{2} + E_{3} [keV]");
    h_fil->GetXaxis()->SetLabelOffset(0.01);
    h_fil->GetXaxis()->SetLabelSize(0.035);
    h_fil->GetXaxis()->SetTitleSize(0.04);
    h_fil->GetXaxis()->SetTitleOffset(1.15);
    h_fil->GetYaxis()->SetTitle("E_{1} [keV]");
    h_fil->GetYaxis()->SetLabelOffset(0.008);
    h_fil->GetYaxis()->SetLabelSize(0.035);
    h_fil->GetYaxis()->SetTitleSize(0.04);
    h_fil->GetYaxis()->SetTitleOffset(1.2);
    h_fil->GetXaxis()->SetRangeUser(700, 1300);
    h_fil->GetYaxis()->SetRangeUser(100, 520);
    h_fil->SetLineWidth(1);
    h_fil->Draw("COLZ2");

   
}

