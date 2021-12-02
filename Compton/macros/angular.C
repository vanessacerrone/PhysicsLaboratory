#include <vector>
#include <string>

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

double myfit(double* x, double* par){
    Double_t b = par[0];
    Double_t a = par[1];

    Double_t fitval = 0;

    fitval = (b * x[0] + a);

    return fitval;
}


vector<string> filelist = {

    "../angular/0deg.root",
    "../angular/20deg.root",
    "../angular/40deg.root",
    "../angular/60deg.root",
    "../angular/70deg.root",
    "../angular/90deg_3.root"
    };


void plot() {

    set_root_style(1);

    slimport_data_t indata_ch0,indata_ch1,indata_ch2;
    TBranch *inbranch_1;
	TBranch *inbranch_2;
    TBranch *inbranch_0;

    int N = 6;

    TTree *intree[6];
    TH1F **h1 = new TH1F*[6]; 
    TH1F **h2 = new TH1F*[6];
    TH1F **h0 = new TH1F*[6];

    TFile *infile[6];

    ULong64_t n_events_ch0[6];
    ULong64_t n_events_ch1[6];
    ULong64_t n_events_ch2[6];
    
    int angles[6] = { 0, 20, 40, 60,70, 90 };


    for(int i=0;i < N;i++){
        infile[i] = new TFile(filelist[i].c_str());
        intree[i] = (TTree*) infile[i]->Get("acq_tree_0");
        inbranch_0 = intree[i]->GetBranch("acq_ch0");
        n_events_ch0[i] = inbranch_0->GetEntries();
	    inbranch_0->SetAddress(&indata_ch0.timetag);
        h0[i] = new TH1F(Form("h1%d",i), Form("Tagger spectrum @ angle = %i#circ",angles[i]),1000,0,1700);

        for(int j=0; j<n_events_ch0[i]; j++){
            inbranch_0->GetEntry(j);
            Double_t entry = (-7.32807 +  0.0544615*indata_ch0.qlong);
            h0[i]->Fill(entry);
        }


    }


    for(int i=0;i < N;i++){
        infile[i] = new TFile(filelist[i].c_str());
        intree[i] = (TTree*) infile[i]->Get("acq_tree_0");
        inbranch_1 = intree[i]->GetBranch("acq_ch1");
        n_events_ch1[i] = inbranch_1->GetEntries();
	    inbranch_1->SetAddress(&indata_ch1.timetag);
        h1[i] = new TH1F(Form("h1%d",i), Form("Scatterer spectrum @ angle = %i#circ",angles[i]),1000,0,1700);

        for(int j=0; j<n_events_ch1[i]; j++){
            inbranch_1->GetEntry(j);
            Double_t entry = (-7.81986 + 0.0577517*indata_ch1.qlong);
            h1[i]->Fill(entry);
        }


    }
    

    for(int i=0;i < N;i++){
        infile[i] = new TFile(filelist[i].c_str());
        intree[i] = (TTree*) infile[i]->Get("acq_tree_0");
        inbranch_2 = intree[i]->GetBranch("acq_ch2");
        n_events_ch2[i] = inbranch_2->GetEntries();
	    inbranch_2->SetAddress(&indata_ch2.timetag);
        h2[i] = new TH1F(Form("h2%d",i), Form("Detector spectrum @ angle = %i#circ",angles[i]),1000,0,1700);

        for(int j=0; j<n_events_ch2[i]; j++){
            inbranch_2->GetEntry(j);
            Double_t entry = (-6.78006 +  0.0531775*indata_ch2.qlong);
            h2[i]->Fill(entry);
        }


    }


    TCanvas* c0 = new TCanvas("c0", "canvas", 550, 1200);
    c0->Divide(2, 3, 0.0005, 0.0005);
    float w0;
    for (int j=0; j < N; j++) {
    w0 = h0[j]->GetBinWidth(0);
    c0->cd(j+1);
    h0[j]->GetXaxis()->SetTitle("Energy [keV]");
    h0[j]->GetYaxis()->SetTitle(Form("Counts/%0.1f keV",w0));
    h0[j]->GetYaxis()->SetMaxDigits(3);
    h0[j]->Draw("same");

        }

    TCanvas* c1 = new TCanvas("c1", "canvas", 550, 1200);
    c1->Divide(2,3, 0.0005, 0.0005);
    float w1;
    for (int j=0; j < N; j++) {
    w1 = h1[j]->GetBinWidth(0);
    c1->cd(j+1);
    h1[j]->GetXaxis()->SetTitle("Energy [keV]");
    h1[j]->GetYaxis()->SetTitle(Form("Counts/%0.1f keV",w1));
    h1[j]->GetYaxis()->SetMaxDigits(3);
    h1[j]->Draw("same");

  }
    
    TCanvas* c2 = new TCanvas("c2", "canvas", 550, 1200);
    c2->Divide(2, 3, 0.0005, 0.0005);
    float w2;
    for (int j=0; j < N; j++) {
    w2 = h2[j]->GetBinWidth(0);
    c2->cd(j+1);
    h2[j]->GetXaxis()->SetTitle("Energy [keV]");
    h2[j]->GetYaxis()->SetTitle(Form("Counts/%0.1f keV",w2));
    h2[j]->GetYaxis()->SetMaxDigits(3);
    h2[j]->Draw("same");

  }


}



