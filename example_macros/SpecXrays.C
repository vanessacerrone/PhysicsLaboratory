#include <fstream>
#include <iostream>
#include <TH1.h>
#include <string>
#include <sstream>
#include <TCanvas.h>

void SpecXrays(string infile){

  TH1I *spec = new TH1I("spec","spec",4096,-.5,4095.5);
  ifstream input;
  input.open(infile.c_str());
  if(!input.good())return;
  string aline, codeword;
  bool reachedData = false;
  int goodline=-1;
  int binContent;

  while(getline(input,aline)){
    if(aline.empty()) continue;
    stringstream oneLine (aline);
    oneLine >> codeword;
    if(codeword.compare(0,4,"DATA") == 0){
      cout <<"Reached the data" << endl;
      reachedData = true;
      getline(input,aline);
      goodline = 0;
      continue;
    }
    if(!reachedData)continue;
    spec->SetBinContent(goodline,atoi(codeword.c_str()));
    goodline++;
  }

  spec->Draw();
  
}
