#include "gethisto.C"
#include <vector>


void filter(const string name_file, int numBins, double minX, double maxX) {
	// variables
	slimport_data_t indata1,indata2;
	TFile *infile = new TFile(name_file.c_str());
	TTree *intree = (TTree*)infile->Get("acq_tree_0");

	TBranch *inbranch1 = intree->GetBranch("acq_ch0");
    TBranch *inbranch2 = intree->GetBranch("acq_ch1");

	inbranch1->SetAddress(&indata1.timetag);
    inbranch2->SetAddress(&indata2.timetag);

	TH1F *h_spectrum1 = new TH1F("h1","Total spectrum",numBins,minX,maxX);
    TH1F *h_spectrum2 = new TH1F("h2","Total spectrum",numBins,minX,maxX);

	// histogram filling
    double entry1,entry2;

	for (int i=0; i<inbranch1->GetEntries(); i++) {
        inbranch2->GetEntry(i);
        entry2 = indata2.qlong;
		inbranch1->GetEntry(i);
        entry1 = indata1.qlong;
        

		if ((entry1 + entry2) < 9661.3673 && (entry1 + entry2) > 7932.7589) {

			h_spectrum1->Fill(entry1);
            h_spectrum2->Fill(entry2);
            
		}

	}

    TCanvas* c1 = new TCanvas("c1","D1",1080,1020);
    h_spectrum1->Draw();
    h_spectrum1->SaveAs("D1_filter.root");
    TCanvas* c2 = new TCanvas("c2","D2",1080,1020);
    h_spectrum2->Draw();
    h_spectrum2->SaveAs("D2_filter.root");


}