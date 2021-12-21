
using namespace std;

//C++ LIBRARIES
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>
//ROOT LIBRARIES
#include "TROOT.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TMath.h"

/*void ReadFile(string name, vector <double> x, vector <double> y)
{
    ifstream f;
    f.open(name);
    double i = 0;
    while(f >> i) 
    {

        x.push_back(i);
        std::cout<<x[i];
        f >> i;
        y.push_back(i); 
    }    

}*/
/*-------- MAIN -------*/

void plot()
{   
    string name="releff.txt";

    vector<double> x, y;

    ifstream f;
    f.open(name);
    double i = 0;
    while(f >> i) 
    {

        x.push_back(i);
        //std::cout<<x[i];
        f >> i;
        y.push_back(i); 
    }    

   
    //ReadFile(name,x,y);
    //ReadFile(name2,x,y);
  /*  for (int j=0; j<4; j++)
    {
        std::cout<< x[j]<<",";
    }
    */
 auto graph = new TGraph(x.size(), &x[0], &y[0]);

 TCanvas* c5 = new TCanvas("c5", "Pd-enriched,  Activity 172 peak versus time for HPGe",
                             27, 50, 1020, 760);
    
    
    TMultiGraph* mg3 = new TMultiGraph();
    mg3->Add(graph);

    graph->SetFillColorAlpha(kBlack,0.4);
    graph->SetMarkerColor(kBlack);
    graph->SetMarkerStyle(8);
    graph->SetMarkerSize(1);

    

    mg3->Draw("APE4");

    //std::cout<<"zero projection "<<f1->Eval(0)<<std::endl;
    mg3->SetTitle("Plot of relative efficiencies for HPGe");
    mg3->GetYaxis()->SetTitle("Efficiency");
    mg3->GetXaxis()->SetTitle("E [keV]");


   /*auto legend = new TLegend(0.1,0.7,0.48,0.9);
   legend->SetHeader("Legend","C"); // option "C" allows to center the header
   legend->AddEntry(graph,"Calibration data","p");
   legend->AddEntry(graph1,"2-photons data","p");
   legend->AddEntry(graph2,"3-photons data","p");
   legend->Draw("same");*/
   return;
}
