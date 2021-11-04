#include <string>
#include "../include/TFile.h"
#include "../include/TH1F.h"
#include "../include/TTreeReaderValue.h"
#include "../include/TTreeReader.h"


void PlotCal(std::string filename)
{
    TFile * input_file = new TFile(filename.c_str());
    TH1F * h = new TH1F("h","h_spectrum;1",16384,0,16384);
    //h->GetXaxis()->Set(16384,0,-17.58+16384*0.11566); 
    h->Draw();
}