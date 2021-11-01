/*
This macro has to be executed only if you are acquiring with at least 2 channels.
It creates a tree, which is essentially a clone of the input one, in a new root file
after fixing possible missing coincidences between different acquisition channels.
For any problems or doubts contact Franco Galtarossa (franco.galtarossa@lnl.infn.it).
*/

struct slimport_data_t {
    ULong64_t	timetag;
    UInt_t	baseline;
    UShort_t	qshort;
    UShort_t	qlong;
    UShort_t	pur;
    UShort_t	samples[4096];
};

void fix_coinc(const char *name_infile, const char *name_outfile){
    
    slimport_data_t indataA, indataB, indataC, indataD;
    int offsetA=0, offsetB=0, offsetC=0, offsetD=0, ent=0;
    int tolerance = 25; // in timestamp units
    TFile *infile = new TFile(name_infile);
    TTree *intree = (TTree*)infile->Get("acq_tree_0");
    
//If you are using channels in a different order change here!
    TBranch *inbranchA = intree->GetBranch("acq_ch0");
    TBranch *inbranchB = intree->GetBranch("acq_ch1");
    TBranch *inbranchC = intree->GetBranch("acq_ch2");
    TBranch *inbranchD = intree->GetBranch("acq_ch3");
    inbranchA->SetAddress(&indataA.timetag);
    inbranchB->SetAddress(&indataB.timetag);
    inbranchC->SetAddress(&indataC.timetag);
    inbranchD->SetAddress(&indataD.timetag);

    TFile *outfile = new TFile(name_outfile, "RECREATE");
    TTree *outtree = intree->CloneTree(0);

 if(inbranchC->GetEntries()==0 && inbranchD->GetEntries()==0){ // If you are acquiring with just 2 channels (ch0 and ch1)
                                                      
     while(ent+offsetA<inbranchA->GetEntries() && ent+offsetB<inbranchB->GetEntries()){
         inbranchA->GetEntry(ent+offsetA);
         inbranchB->GetEntry(ent+offsetB);
         if(indataA.timetag < indataB.timetag - tolerance){
	     offsetA++;
	     continue;
	 } else if(indataA.timetag > indataB.timetag + tolerance){
	     offsetB++;
	     continue;
	 }
                                                         
         ent++;                                                                                         
         outtree->Fill();
                                                          
         if(ent == 1) cout << endl << "Let's fix the coincidences!" << endl << endl;
         if(ent % 25000 == 0 && ent != 0){
             cout << ent << " entries analyzed. Only " << inbranchA->GetEntries() - ent << " entries left." << endl;
             cout.flush();
         }
                                                         
     }

 } else if(inbranchD->GetEntries()==0){ // If you are acquiring with 3 channels (ch0, ch1 and ch2)


     while(ent+offsetA<inbranchA->GetEntries() && ent+offsetB<inbranchB->GetEntries() && ent+offsetC<inbranchC->GetEntries()){
        inbranchA->GetEntry(ent+offsetA);
        inbranchB->GetEntry(ent+offsetB);
        inbranchC->GetEntry(ent+offsetC);
        if(indataA.timetag < indataB.timetag - tolerance || indataA.timetag < indataC.timetag - tolerance){
            offsetA++;
            continue;
        } else if(indataB.timetag < indataA.timetag - tolerance || indataB.timetag < indataC.timetag - tolerance){
            offsetB++;
            continue;
        } else if(indataC.timetag < indataA.timetag - tolerance || indataC.timetag < indataB.timetag - tolerance){
            offsetC++;
            continue;
        }

        ent++;
        outtree->Fill();
        
        if(ent == 1) cout << endl << "Let's fix the coincidences!" << endl << endl;
        if(ent % 25000 == 0 && ent != 0){
            cout << ent << " entries analyzed. Only " << inbranchA->GetEntries() - ent << " entries left." << endl;
            cout.flush();
        }

     }
     
 } else { // If you are acquiring with 4 channels (ch0, ch1, ch2 and ch3)

    while(ent+offsetA<inbranchA->GetEntries() && ent+offsetB<inbranchB->GetEntries() && ent+offsetC<inbranchC->GetEntries() && ent+offsetD<inbranchD->GetEntries()){
        
        inbranchA->GetEntry(ent+offsetA);
        inbranchB->GetEntry(ent+offsetB);
        inbranchC->GetEntry(ent+offsetC);
        inbranchD->GetEntry(ent+offsetD);

        if(indataA.timetag < indataB.timetag - tolerance || indataA.timetag < indataC.timetag - tolerance || indataA.timetag < indataD.timetag - tolerance){
            offsetA++;
            continue;
        } else if(indataB.timetag < indataA.timetag - tolerance || indataB.timetag < indataC.timetag - tolerance || indataB.timetag < indataD.timetag - tolerance){
            offsetB++;
            continue;
        } else if(indataC.timetag < indataA.timetag - tolerance || indataC.timetag < indataB.timetag - tolerance || indataC.timetag < indataD.timetag - tolerance){
           	offsetC++;
            continue;
        } else if(indataD.timetag < indataA.timetag - tolerance || indataD.timetag < indataB.timetag - tolerance || indataD.timetag < indataC.timetag - tolerance){
            offsetD++;
            continue;
        }
 
        ent++;
        outtree->Fill();
            
        if(ent == 1) cout << endl << "Let's fix the coincidences!" << endl << endl;
        if(ent % 25000 == 0 && ent != 0){
            cout << ent << " entries analyzed. Only " << inbranchA->GetEntries() - ent << " entries left." << endl;
            cout.flush();
        }
    
    }

 }
    
    outfile->cd();
    outtree->Write();
    outfile->Close();
    delete outfile;

}
