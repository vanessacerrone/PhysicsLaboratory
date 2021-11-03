void three_gaussians(){
    
    TF1 *f1 = new TF1("f1","gaus(0)+gaus(3)+gaus(6)+pol1(9)");
    f1->SetParameters(1000,-10,3,600,1,3,600,10,3,40,.4);
    TH1F *h1 = new TH1F("h1","Three gaussians",200,-50,50);
    h1->FillRandom("f1",100000);
    h1->Draw();

    f1->SetParLimits(0,2000,3000); //constant of first gaussian
    f1->SetParLimits(1,-20,0); //centroid of first gaussian
    f1->SetParLimits(2,0.1,10); //sigma of first gaussian
    f1->SetParLimits(3,1000,2000); //constant of second gaussian
    f1->SetParLimits(4,-10,10); //centroid of second gaussian
    f1->SetParLimits(5,0.1,10); //sigma of second gaussian
    f1->SetParLimits(6,1000,2000); //constant of third gaussian
    f1->SetParLimits(7,0,20); //centroid of third gaussian
    f1->SetParLimits(8,0.1,10); //sigma of third gaussian
    
    h1->Fit("f1");
    cout << "Chi_square/NDF = " << f1->GetChisquare()/f1->GetNDF() << endl;
    
}
