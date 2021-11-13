
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
    string name="dtCal.txt";
    string name2="dt2Gamma.txt";
    string name3="dt3Gamma.txt";

    vector<double> x, y,x1,y1,x2,y2;

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

    ifstream f1;
    f1.open(name2);
    i = 0;
    while(f1 >> i) 
    {

        x1.push_back(i);
        //std::cout<<x[i];
        f1 >> i;
        y1.push_back(i); 
    }    

    ifstream f2;
    f2.open(name3);
    i = 0;
    while(f2 >> i) 
    {

        x2.push_back(i);
        //std::cout<<x[i];
        f2 >> i;
        y2.push_back(i); 
    }    
    //ReadFile(name,x,y);
    //ReadFile(name2,x,y);
  /*  for (int j=0; j<4; j++)
    {
        std::cout<< x[j]<<",";
    }
    */
 auto graph = new TGraph(x.size(), &x[0], &y[0]);
 auto graph1 = new TGraph(x1.size(), &x1[0], &y1[0]);
 auto graph2 = new TGraph(x2.size(), &x2[0], &y2[0]);

 TCanvas* c5 = new TCanvas("c5", "Pd-enriched,  Activity 172 peak versus time for HPGe",
                             27, 50, 1020, 760);
    
    
    TMultiGraph* mg3 = new TMultiGraph();
    mg3->Add(graph);
    mg3->Add(graph1);
    mg3->Add(graph2);

    graph->SetFillColorAlpha(kRed-7,0.4);
    graph->SetMarkerColor(kRed);
    graph->SetMarkerStyle(8);
    graph->SetMarkerSize(1);

    graph1->SetFillColorAlpha(kBlue,0.4);
    graph1->SetMarkerColor(kBlue);
    graph1->SetMarkerStyle(8);
    graph1->SetMarkerSize(1);

    graph2->SetFillColorAlpha(kBlue,0.4);
    graph2->SetMarkerColor(kGreen);
    graph2->SetMarkerStyle(8);
    graph2->SetMarkerSize(1);

    mg3->Draw("APE4");

    //std::cout<<"zero projection "<<f1->Eval(0)<<std::endl;
    mg3->SetTitle("Dead Time");
    mg3->GetYaxis()->SetTitle("Lost events [%]");
    mg3->GetXaxis()->SetTitle("Rate Scaler");


   auto legend = new TLegend(0.1,0.7,0.48,0.9);
   legend->SetHeader("Legend","C"); // option "C" allows to center the header
   legend->AddEntry(graph,"Calibration data","p");
   legend->AddEntry(graph1,"2-photons data","p");
   legend->AddEntry(graph2,"3-photons data","p");
   legend->Draw("same");
   return;
}
