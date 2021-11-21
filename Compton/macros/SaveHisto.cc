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
	slimport_data_t indata_ch2;
	slimport_data_t indata_ch3;


	TFile *infile = new TFile(infilename.c_str());
	TTree *intree = (TTree*) infile->Get("acq_tree_0");

	TBranch *inbranch_0;
	TBranch *inbranch_1;
	TBranch *inbranch_2;
	TBranch *inbranch_3;

	// Digitizer CH0
	inbranch_0 = intree->GetBranch("acq_ch0");
	inbranch_0->SetAddress(&indata_ch0.timetag);

	// Digitizer CH1
	inbranch_1 = intree->GetBranch("acq_ch1");
	inbranch_1->SetAddress(&indata_ch1.timetag);

	// Digitizer CH2
	inbranch_2 = intree->GetBranch("acq_ch2");
	inbranch_2->SetAddress(&indata_ch2.timetag);

	// Digitizer CH3
	inbranch_3 = intree->GetBranch("acq_ch3");
	inbranch_3->SetAddress(&indata_ch3.timetag);


	psd_params_t params;
	TBranch *settings = intree->GetBranch("psd_params");
	settings->SetAddress(&params.channel);
	settings->GetEntry(0);

	ULong64_t n_events_ch0 = inbranch_0->GetEntries();
	ULong64_t n_events_ch1 = inbranch_1->GetEntries();
	ULong64_t n_events_ch2 = inbranch_2->GetEntries();
	ULong64_t n_events_ch3 = inbranch_3->GetEntries();

	ULong64_t counter_ch0 = 0;
	ULong64_t counter_ch1 = 0;
	ULong64_t counter_ch2 = 0;
	ULong64_t counter_ch3 = 0;


    Int_t nbins = 1000;
    Double_t xmin = 0;
    Double_t xmax = 20000;


	// CH0 spectrum
	TH1F *ch0_spectrum = new TH1F("ch0", "Channel 0 spectrum", nbins, xmin, xmax);
	ch0_spectrum->GetXaxis()->SetTitle("ADC counts");
	ch0_spectrum->GetYaxis()->SetTitle("Counts");

	// CH1 spectrum
	TH1F *ch1_spectrum = new TH1F("ch1", "Channel 1 spectrum", nbins, xmin, xmax);
    ch1_spectrum->GetXaxis()->SetTitle("ADC counts");
	ch1_spectrum->GetYaxis()->SetTitle("Counts");

	// CH2 spectrum
	TH1F *ch2_spectrum = new TH1F("ch2", "Channel 2 spectrum", nbins, xmin, xmax);
	ch2_spectrum->GetXaxis()->SetTitle("ADC counts");
	ch2_spectrum->GetYaxis()->SetTitle("Counts");


	// CH3 spectrum
	TH1F *ch3_spectrum = new TH1F("ch3", "Channel 3 spectrum", nbins, xmin, xmax);
	ch3_spectrum->GetXaxis()->SetTitle("ADC counts");
	ch3_spectrum->GetYaxis()->SetTitle("Counts");

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

	// Fill histogram for channel 2
	while( counter_ch2 < n_events_ch2)
	{

		inbranch_2->GetEntry(counter_ch2);
		ch2_spectrum->Fill(indata_ch2.qlong); 
		counter_ch2++;

	}

	// Fill histogram for channel 2
	while( counter_ch3 < n_events_ch3)
	{

		inbranch_3->GetEntry(counter_ch3);
		ch3_spectrum->Fill(indata_ch3.qlong);
		counter_ch3++;

	}
    
	// Saving Histograms into a root file
	TFile * outfile = new TFile(outfilename.c_str(), "RECREATE");

	ch0_spectrum->Write();
	ch1_spectrum->Write();
	ch2_spectrum->Write();
	ch3_spectrum->Write();

	outfile->Close();

	return ;
}