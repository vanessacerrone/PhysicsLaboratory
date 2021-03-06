/*-- Set RootStyle *--/

/*
 * Author      : Vanessa
 * Date        : 20211120
 * Usage       : #include "RootStyle.cc" in your macro 
 *                if n = 0 histograms are plotted in the usual way (no fill, blue lines)
 *                if n !=0 histograms are plotted with Azure fill and black contour lines  
*/

#include <TROOT.h>
#include <TStyle.h>


void set_root_style(int n)
{
  gStyle->SetTextFont(22);
  // Canvas
  gStyle->SetCanvasColor(0);
  gStyle->SetCanvasBorderSize(0);
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetCanvasDefH(900); // Height of canvas
  gStyle->SetCanvasDefW(900); // Width of canvas
 
  
  // Pads
  gStyle->SetPadColor(0);
  gStyle->SetPadBorderSize(10);
  gStyle->SetPadBorderMode(0);
  gStyle->SetPadBottomMargin(0.1);
  gStyle->SetPadTopMargin(0.1);
  gStyle->SetPadLeftMargin(0.11);
  gStyle->SetPadRightMargin(0.11);
  gStyle->SetPadGridX(0);
  gStyle->SetPadGridY(0);
  gStyle->SetPadTickX(0);
  gStyle->SetPadTickY(0);
  gStyle->SetTitleY(0.97);
  

  
  // Histograms
  if(n == 0) {   
      gStyle->SetHistFillColor(kWhite);
      gStyle->SetHistLineColor(kBlue+2);

    }
 
  else {   
      gStyle->SetHistFillColor(kAzure-9);
      gStyle->SetHistLineColor(kBlack);
      gStyle->SetHistLineWidth(1);
    }

  gStyle->SetHistLineStyle(1);
  gStyle->SetHistLineWidth(1);

  
  // 2D histo
  gStyle->SetNumberContours(70);
  gStyle->SetPalette(57);
  
  // Markers
  gStyle->SetMarkerStyle(20); 
  gStyle->SetMarkerSize(0.8);
  
  // Various
  gStyle->SetTickLength(0.02,"xy");    
  gStyle->SetTitleSize(0.04,"XYZ");
  gStyle->SetTitleFont(42,"XYZ");
  gStyle->SetLabelFont(42,"XYZ");
  gStyle->SetNdivisions(509,"XY");

  gStyle->SetTitleOffset(1.15,"X");
  gStyle->SetTitleOffset(1.4,"Y");
  gStyle->SetTitleOffset(1,"Z");
  

  gStyle->SetLabelOffset(0.01,"X");
  gStyle->SetLabelOffset(0.008,"Y");
  gStyle->SetLabelOffset(0.015,"Z");
  
  gStyle->SetLabelSize(0.04,"X");
  gStyle->SetLabelSize(0.04,"Y");
  gStyle->SetLabelSize(0.04,"Z");
  
  gStyle->SetStatFont(42);
  gStyle->SetTitleFont(42);
  
  // Options
  gStyle->SetOptFit(0);
  gStyle->SetOptStat(0);
  
  gROOT->ForceStyle();
  
  return;
}
