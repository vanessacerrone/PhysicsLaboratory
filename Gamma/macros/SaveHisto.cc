/*
/	Read digitizer data from the LAB
/	Digitizer: CAEN
/   NB: for Compton we'll use just ch0,1,2 
/
/   11/20/2021  
*/

#include <iostream>
#include <string>
#include "RootStyle.cc"

struct slimport_data_t {

	ULong64_t	timetag;
	UInt_t		baseline;
	UShort_t	qshort;
	UShort_t	qlong;
	UShort_t	pur;
	UShort_t	samples[4096];
};


struct psd_params_t {
	UInt_t		channel;
	UInt_t		threshold;
	UInt_t		pretrigger;
	UInt_t		pregate;
	UInt_t		shortgate;
	UInt_t		longgate;
	UInt_t		numsamples;
};


void save_histo(string infilename, string outfilename)
{
    set_root_style(1);

	slimport_data_t indata_ch0;
	slimport_data_t indata_ch1;



	TFile *infile = new TFile(infilename.c_str());
	TTree *intree = (TTree*) infile->Get("acq_tree_0");

	TBranch *inbranch_0;
	TBranch *inbranch_1;

	// Digitizer CH0
	inbranch_0 = intree->GetBranch("acq_ch0");
	inbranch_0->SetAddress(&indata_ch0.timetag);

	// Digitizer CH1
	inbranch_1 = intree->GetBranch("acq_ch1");
	inbranch_1->SetAddress(&indata_ch1.timetag);



	psd_params_t params;
	TBranch *settings = intree->GetBranch("acq_params");
	settings->SetAddress(&params.channel);
	settings->GetEntry(0);

	ULong64_t n_events_ch0 = inbranch_0->GetEntries();
	ULong64_t n_events_ch1 = inbranch_1->GetEntries();

	
	ULong64_t counter_ch0 = 0;
	ULong64_t counter_ch1 = 0;



    Int_t nbins = 1000;
    Double_t xmin = 0;
    Double_t xmax = 30000;


	// CH0 spectrum
	TH1F *ch0_spectrum = new TH1F("ch0", "Na(I) spectrum", nbins, xmin, xmax);
	ch0_spectrum->GetXaxis()->SetTitle("ADC counts");
	ch0_spectrum->GetYaxis()->SetTitle("Counts");

	// CH1 spectrum
	TH1F *ch1_spectrum = new TH1F("ch1", "HPGe spectrum", 2500, 0,25000);
    ch1_spectrum->GetXaxis()->SetTitle("ADC counts");
	ch1_spectrum->GetYaxis()->SetTitle("Counts");


	// Fill histogram for channel 0
	while( counter_ch0 < n_events_ch0)
	{

		inbranch_0->GetEntry(counter_ch0);
		ch0_spectrum->Fill(indata_ch0.qlong);
		counter_ch0++;

	}

	// Fill histogram for channel 1
	while( counter_ch1 < n_events_ch1)
	{

		inbranch_1->GetEntry(counter_ch1);
		ch1_spectrum->Fill(indata_ch1.qlong);
		counter_ch1++;

	}

    
	// Saving Histograms into a root file
	TFile * outfile = new TFile(outfilename.c_str(), "RECREATE");

	ch0_spectrum->Write();
	ch1_spectrum->Write();


	outfile->Close();

	return ;
}