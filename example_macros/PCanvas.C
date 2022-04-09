//*-- Author :Marcello Lunardon, Phool Chand
//*CMZ :      M. Lunardon
/////////////////////////////////////////////////////////////////////////////////////////
//
//      The 8plp Canvas
//      ===============
//
//   The PCanvas class is an example of a TCanvas class extended with other 
//   variables and with our own mouse and keyboard events handling. This class
//   inherits completely from the TCanvas and overwrites the HandleInput()
//   and other few methods;
//
//   You can use the PCanvas for:
//  
//   - handle TCutG defined in a 2D histogram ( PrintCutG(),GetCutG(),
//     DeleteCutG(), CalVol() );
//
//   - integrate and fit peaks and backgrounds in a 1D histogram
//     in Trackn style:
//
//     'i'  = integral (2)
//     'b'  = bkg (4 = 2 pairs, left and right)
//     'cj' = calculate integral with bkg subtraction (if b markers defined)
//     'cb' = calculate background
//
//     'g'  = peak positions
//     'r'  = fit region
//     'cg' = calculate multigaussian fit
//
//     'dq' = define matrix (bidim)
//     'cw' = calculate slice with bkg subtraction
//     'b'  = background regione in main axis projection
//     'i'  = gate for slice in the main axis projection
//
//     'z'+'x' = delete marker of type x
//     'm'+'x' = show marker of type x
//
//   - with button 2 of the mouse it's possible to load/save ascii
//     histogram/graphics, smooth histograms and more
//
//////////////////////////////////////////////////////////////////////////////////////////


#include "TROOT.h"
#include "TSystem.h"
#include "TApplication.h"
#include "TVirtualX.h"
#include "TGClient.h"
#include "TGButton.h"
#include "TGTextEntry.h"
#include "TGComboBox.h"
#include "TGLabel.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TCanvasImp.h"
#include "TControlBar.h"
#include "TH1.h"
#include "TF1.h"
#include "TH2.h"
#include "TFrame.h"
#include "TLine.h"
#include "TCutG.h"
#include "TPaveText.h"
#include "TLatex.h"
#include "TGMenu.h"
#include "TGWindow.h"
#include <fstream>      // std::ifstream
#include "TRint.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <TROOT.h>
#include <RQ_OBJECT.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TH1.h>
#include <TGraph.h>
#include <TRandom.h>
#include <TSystem.h>
#include <TApplication.h>
#include <TVirtualX.h>
#include <TGClient.h>
#include <TGButton.h>
#include <TGTextEntry.h>
#include <TGComboBox.h>
#include <TCanvas.h>
#include <TCanvasImp.h>
#include <TGuiFactory.h>
#include <TGMenu.h>
#include <TH1.h>
#include <TGFrame.h>
#include <TGFileDialog.h>
#include <TFile.h>
#include <TInterpreter.h>
#include <TEnv.h>
#include <TGMsgBox.h>
#include <TGResourcePool.h>
#include <TArrayL.h>
#include <TGLabel.h>

//--------------------------------------------------------------------------
// header file
//--------------------------------------------------------------------------

#define  MAXPGLINES 100
#define  MAXENTRIES 100

enum EPGInputButtonType {
   kPG_Nothing       = 0,
   kPG_OK            = BIT(0),
   kPG_CANCEL        = BIT(1),
   kPG_APPLY         = BIT(2),
   kPG_REFRESH       = BIT(3),
   kPG_QUIT          = BIT(4),
   kPG_BIT5          = BIT(5),
   kPG_BIT6          = BIT(6),
   kPG_BIT7          = BIT(7),
   kPG_ALL           = (kPG_OK | kPG_CANCEL | kPG_APPLY | kPG_REFRESH)
};

class PGInput;

//--------------------------------------------------------------------
class PGInputWindow : public TGTransientFrame {
  friend class PGInput;

RQ_OBJECT("PGInputWindow")

 protected:
  
  TGLayoutHints	*fLLeft;
  TGLayoutHints	*fLCol;
  TGLayoutHints	*fLMain;
  TGLayoutHints	*fLRight;
  TGLayoutHints	*fLCenter;
  TGLayoutHints	*fLNormal;
  
  TGCompositeFrame *fF1[MAXPGLINES];
  TGCompositeFrame *fSpace[MAXPGLINES];
  TGCompositeFrame *fFinal;
  TGCompositeFrame *fCentral;
  TGCompositeFrame *fCol[20];
  Int_t         fNcol;
  
  TGLabel	*fLabel[MAXPGLINES];
  TGTextEntry	*fTextEntry[MAXPGLINES];
  TGTextBuffer	*fTbuf[MAXPGLINES];
  TGTextButton	*fButton[MAXPGLINES];
  
  TGButton	*fButtonOK;
  TGButton	*fButtonCANCEL;
  TGButton	*fButtonAPPLY;
  TGButton	*fButtonREFRESH;
  TGButton	*fButtonQUIT;
  
  TGComboBox    *fCombo[MAXPGLINES];
  TGCheckButton *fCheck[MAXPGLINES];

  TGGC           fTextGC;
  TGGC           fInfoGC;
  TGGC           fCommentGC;
  
  PGInput       *pgi;
  
  PGInputWindow(const TGWindow* p, const TGWindow *main, UInt_t w, UInt_t h,
		PGInput *pg, PGInput *usub=NULL, UInt_t options = kMainFrame | kVerticalFrame);
  virtual ~PGInputWindow();
  virtual  void  CloseWindow();
  Bool_t   ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);

  void UpdateData(Int_t idx=-1); // -1=all fields
  void UpdateField(Int_t idx=-1); // -1=all fields
  void ProcessButton(Long_t parm1);
  void ProcessCommand(Int_t idx);

 public:
  PGInput *usersubptr;

// #ifdef ROOT5
//   ClassDef(PGInputWindow,0)
// #endif

};


class PGInput {
  friend class PGInputWindow;

RQ_OBJECT("PGInput")

  protected:
  PGInputWindow *fWin;
  PGInput       *fParente; //!  do not stream
  PGInput       *fUsersubPtr; //!  do not stream
  Int_t          fSizeX;
  Int_t          fSizeY;
  Int_t          fPx; // number of X pixels for a char 
  Int_t          fPy; // number of Y pixels for a char 
  Int_t          fLenghtQ;
  Int_t          fLenghtA;
  Bool_t         fIsWin;
  Int_t          fNoItems;
  Char_t         fTitle[200];
  Char_t         fQuestion[MAXPGLINES][80];
  Char_t         fAnswer[MAXPGLINES+5][160];
  Int_t          fLenght[MAXPGLINES+5];
  Char_t         fType[MAXPGLINES]; // s=string, i=Int_t, f=Float_t, d=Double_t
                                    // b=Bool_t, l=Long_t, h=Short_t, c=Char_t, v=bool
  Int_t          fWidget[MAXPGLINES]; // 1=edit box 2=combo ...

  TGString       *fEntryList[MAXPGLINES]; // entry list for combo new

  Char_t         fFileType[MAXPGLINES][12]; // def. ext.
  Bool_t         fIsOK;
  Bool_t         fIsAPPLY;
  Bool_t         fIsCANCEL;
  Bool_t         fIsREFRESH;
  Bool_t         fIsQUIT;
  Int_t          fButChoice[MAXPGLINES+5];
  TArrayL        fButPar[MAXPGLINES+5];
  Int_t          fComboSelection[MAXPGLINES];
  Int_t          fComboNoEntries[MAXPGLINES];
  Int_t          fCol[MAXPGLINES];
  Long_t         fAddr[MAXPGLINES];
  Int_t          fInfoLine[MAXPGLINES];
  Int_t          fNoInfo;
  Bool_t         fWait;
  Int_t          fRetval;
  Char_t         fFgColor[20];
  Char_t         fBgColor[20];
  Char_t         fButtonColor[20];
  Char_t         fInfoColor[20];
  Char_t         fCommentColor[20];

  void CalculateSize();
  void SetIsWin(Bool_t w) {fIsWin=w;}
  void SetButtons(EPGInputButtonType b);

  FontStruct_t   fontsel;

 public:

  PGInput();
  virtual ~PGInput();

  Int_t    GetNoItems() {return fNoItems;}
  Int_t    GetPixelX() {return fPx;}
  Int_t    GetPixelY() {return fPy;}
  void     SetPixelX(Int_t n) {fPx = n;}
  void     SetPixelY(Int_t n) {fPy = n;}
  ULong_t  GetInfoColor(); 
  //{return fInfoColor;}
  ULong_t  GetFgColor(); 
  //{return fFgColor;}
  ULong_t  GetBgColor(); 
  //{return fBgColor;}
  ULong_t  GetCommentColor(); 
  //{return fCommentColor;}
  ULong_t  GetButtonColor(); 
  //{return fButtonColor;}

  void     SetFont(const Char_t *name);

  void     SetInfoColor(const Char_t *colorname);
  void     SetFgColor(Char_t *colorname);
  void     SetBgColor(Char_t *colorname);
  void     SetCommentColor(const Char_t *colorname);
  void     SetButtonColor(const Char_t *colorname);
  void     SetTitle(const Char_t *title) {strcpy(fTitle,title);}
  Char_t  *GetTitle() {return fTitle;}
  Bool_t   IsWin() {return fIsWin;}
  void     UpdateData(Int_t idx=-1); // -1=all fields
  void     UpdateField(Int_t idx=-1); // -1=all fields
  Int_t    GetNoComboEntries(Int_t idx);

  void     SetOkAction(const Char_t *command=NULL, Int_t choice=0, 
		       Long_t *par=NULL, Int_t when=1);
  void     SetApplyAction(const Char_t *command=NULL, Int_t choice=0, 
			  Long_t *par=NULL, Int_t when=1);
  void     SetRefreshAction(const Char_t *command=NULL, Int_t choice=0, 
			    Long_t *par=NULL, Int_t when=1);

  void     SetParent(PGInput *parent = NULL) { fParente=parent; }
  PGInputWindow* GetPGWindow() { return fWin; }
  void     RefreshComboList(Int_t idx=-1); // -1=all fields

  Int_t AddEditBox(const Char_t *question, Char_t *string, 
		   Int_t lenght=0, Int_t column=1); // accept a string for input
  Int_t AddEditBox(const Char_t *question, Int_t& num, 
		   Int_t lenght=0, Int_t column=1); // accept an integer for input
  Int_t AddEditBox(const Char_t *question, Short_t& num, 
		   Int_t lenght=0, Int_t column=1); // accept a short for input
  Int_t AddEditBox(const Char_t *question, Long_t& num, 
		   Int_t lenght=0, Int_t column=1); // accept a long for input
  Int_t AddEditBox(const Char_t *question, Float_t& num, 
		   Int_t lenght=0, Int_t column=1); // accept a float for input
  Int_t AddEditBox(const Char_t *question, Double_t& num, 
		   Int_t lenght=0, Int_t column=1); // accept a double for input
  Int_t AddEditBox(const Char_t *question, Char_t& c, Int_t column=1); // accept a char for input
  Int_t AddEditBox(const Char_t *question, Bool_t& yn, 
		   Int_t column=1, int autoupdate=0, const Char_t *command=NULL, Int_t choice=0); // add a check item, eventually with associate action

  Int_t AddComboBox(const Char_t *question, TGString *entries, 
		    Int_t& selection, const Char_t *command=NULL,
		    Int_t choice=0, Long_t *par=NULL, Int_t column=1); // add a combo box

  Int_t AddOpenFile(const Char_t *question, const Char_t *filename, const Char_t *defext="root", 
		    Int_t lenght=0, Int_t column=1); // accept a filename

  Int_t AddButton(const Char_t *question, const Char_t *command, Int_t choice=0, 
		  Long_t *par=NULL, Int_t column=1, 
		  Int_t before=0); // command line button

  Int_t AddComment(const Char_t *comment, Int_t column=1, const Char_t *layout="c"); // command line button

  Int_t AddInfo(const Char_t *info, Int_t shift=20, 
		Int_t column=1, Int_t len=0); // Info line
  Int_t SendInfo(const Char_t *info, Int_t line, bool autoupdate=1);

  Bool_t inpYN(const Char_t *question);
  Int_t  MessageBox(const Char_t *msg);

  Int_t Show(EPGInputButtonType b = kPG_ALL, Bool_t waitfor=0);
  void Close() {if (fIsWin) delete fWin;}
  void Reset();

  TGTextButton	*GetButton(Int_t idx);
  TGCheckButton *GetCheckButton(Int_t idx);
  TGCompositeFrame *GetFrame(Int_t idx);
  void ChangeButtonFace(Int_t idx, Char_t *text=NULL, Char_t *color=NULL,
			Int_t w=0, Int_t h=0);
  //void ChangeCommentColor(Int_t idx, Char_t *color);

  Int_t InputFilename(Char_t *fname, const Char_t *ext="root", const Char_t *mode="open"); // open/save file selector
  Int_t InputFilenames(Char_t *fname, const Char_t *ext1="root", const Char_t *ext2="", const Char_t *ext3="", const Char_t *mode="open"); // open/save file selector

  void SetUsersubPtr(PGInput *pg=NULL); // set the address of the PGInput-derived custom class that overwrite UsersubX

  virtual void Usersub1(Int_t choice=0, Long_t *par=NULL);
  //virtual void Usersub1(Int_t choice, Long_t *par) = 0; // pure virtual - works in root 6
  virtual void Usersub2(Int_t choice=0, Long_t *par=NULL);
  virtual void Usersub3(Int_t choice=0, Long_t *par=NULL);
  virtual void Usersub4(Int_t choice=0, Long_t *par=NULL);
  virtual void Usersub5(Int_t choice=0, Long_t *par=NULL);
  virtual void Usersub6(Int_t choice=0, Long_t *par=NULL);
  virtual void Usersub7(Int_t choice=0, Long_t *par=NULL);
  virtual void Usersub8(Int_t choice=0, Long_t *par=NULL);
  virtual void Usersub9(Int_t choice=0, Long_t *par=NULL);

// #ifdef ROOT5
//   ClassDef(PGInput,1) // 8pLP graphic input utility class
// #endif

};

//-------------------------------------------------------------------------



//--------------------------------------------------------------------------
// class PGInputWindow
//--------------------------------------------------------------------------
PGInputWindow::PGInputWindow(const TGWindow* p, const TGWindow *main, UInt_t w,
			     UInt_t h, PGInput *pg, PGInput *usub, UInt_t options)
  : TGTransientFrame(p,main, w, h, options),
    fTextGC(TGButton::GetDefaultGC()),
    fInfoGC(TGButton::GetDefaultGC()),
    fCommentGC(TGButton::GetDefaultGC())
{
  for (Int_t jj=0; jj<MAXPGLINES; jj++) {
    fF1[jj]=NULL;
    fSpace[jj]=NULL;
    fLabel[jj]=NULL;
    fTextEntry[jj]=NULL;
    fTbuf[jj]=NULL;
    fButton[jj]=NULL;
    fCombo[jj]=NULL;
    fCheck[jj]=NULL;    
  }
  for (Int_t jj=0; jj<10; jj++) {
    fCol[jj]=NULL;
  }
  pgi = pg;
  usersubptr=usub;
  GCValues_t   gval;
  FontStruct_t labelfont;
  labelfont = fClient->GetFontByName(gEnv->GetValue("Gui.NormalFont",
   "-adobe-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1"));
  //labelfont = fClient->GetFontByName(gEnv->GetValue("Gui.NormalFont",
  //   "-misc-fixed-medium-*-normal-*-32-*-*-*-*-*-*-*"));
   // Define new graphics context. Only the fields specified in
   // fMask will be used (for default TGLabel context see
   // http://root.cern.ch/root/html/src/TGClient.cxx.html).
  if (pgi->fontsel)
    labelfont = pgi->fontsel;

  gval.fMask = kGCForeground | kGCFont;
  gval.fFont = gVirtualX->GetFontHandle(labelfont);
  gval.fForeground = pgi->GetFgColor();
  gval.fBackground = pgi->GetBgColor();
  //fTextGC = gVirtualX->CreateGC(fClient->GetRoot()->GetId(), &gval);
  gval.fForeground = pgi->GetInfoColor();
  //fInfoGC = gVirtualX->CreateGC(fClient->GetRoot()->GetId(), &gval);
  gval.fForeground = pgi->GetCommentColor();
  //fCommentGC = gVirtualX->CreateGC(fClient->GetRoot()->GetId(), &gval);
  
  //SetLayoutManager(new TGMatrixLayout(this,1,5) ); 
  //SetLayoutManager(new TGHorizontalLayout(this) );  
  // layout a matrice 3x3: quando e' attivo questo non ci fa piu' nulla il resto
  //SetLayoutManager(new TGMatrixLayout(this,3,3) ); 
  
  fLLeft   = new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 0, 0, 0, 0);
  fLCol    = new TGLayoutHints(kLHintsTop | kLHintsLeft, 2,2,2,2);
  fLRight  = new TGLayoutHints(kLHintsRight | kLHintsCenterY, 0, 0, 0, 0);
  fLMain   = new TGLayoutHints(kLHintsCenterX, 5, 5, 5, 5);
  fLCenter = new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 0, 0, 0, 0);
  fLNormal = new TGLayoutHints(kLHintsNormal | kLHintsCenterY, 5, 5, 5, 5);  

  Int_t nentries=0;
  Int_t j=0;
  Int_t l;

  fCentral = new TGCompositeFrame(this,  60, 20, kHorizontalFrame);
  fCentral->SetBackgroundColor(pgi->GetBgColor());

  // check for column number and define columns
  Int_t ncol=0;
  for (Int_t i=0; i<pgi->GetNoItems(); i++) {
    if (pgi->fCol[i] > ncol) ncol = pgi->fCol[i];
  }
  ncol++;
  fNcol = ncol;

  for (Int_t i=0; i<ncol; i++) {
    fCol[i] = new TGCompositeFrame(fCentral, 60, 60, kVerticalFrame);
    fCol[i]->SetBackgroundColor(pgi->GetBgColor());
  }

  Int_t maxF1[10];
  Int_t maxlabel[10];
  for (Int_t i=0; i<10; i++) {
    maxF1[i]=0;
    maxlabel[i]=0;
  }

  for (Int_t it=0; it<pgi->GetNoItems(); it++) {    
    Int_t colu = pgi->fCol[it]; // column number
    Int_t lque=0;
    
    switch (pgi->fWidget[it]) {
      
    case 1: // edit box
      fF1[it] = new TGCompositeFrame(fCol[colu],  100, pgi->GetPixelY(), 
				     kHorizontalFrame | kFixedHeight);
      fF1[it]->SetBackgroundColor(pgi->GetBgColor());
      
      // add space
      fSpace[it] = new TGCompositeFrame(fF1[it], 0, 20,
					kFixedHeight | kFixedWidth);
      
      fF1[it]->AddFrame(fSpace[it], fLLeft);
      
      // add question
      fLabel[it] = new TGLabel(fF1[it], new TGString(pgi->fQuestion[it]), 
			       fTextGC(), labelfont);
			       //myGC(), labelfont);
      fLabel[it]->SetBackgroundColor(pgi->GetBgColor());
      fLabel[it]->Resize(fLabel[it]->GetDefaultWidth(),
			 fLabel[it]->GetDefaultHeight());
      
      fF1[it]->AddFrame(fLabel[it], fLLeft);
      lque = fLabel[it]->GetWidth();
      //printf("label=%d\n",lque);
      fF1[it]->Resize(fF1[it]->GetDefaultWidth(),pgi->GetPixelY());
      lque = fF1[it]->GetWidth();
      //printf("space+label=%d\n",lque);      
      if (maxlabel[colu] < lque) maxlabel[colu] = lque;
      
      // add answer field
      fTbuf[it] = new TGTextBuffer(100); // dimensione buffer
      fTextEntry[it] = new TGTextEntry(fF1[it], fTbuf[it]);
      //fTextEntry[it]->SetFont("-misc-fixed-medium-*-normal-*-20-*-*-*-*-*-*-*");
      fTextEntry[it]->SetFont("-adobe-courier-bold-r-*-*-14-*-*-*-*-*-iso8859-1");
      l = pgi->fLenght[it]*pgi->GetPixelX();
      fTextEntry[it]->Resize(l, fTextEntry[it]->GetDefaultHeight());
      fTbuf[it]->AddText(0, pgi->fAnswer[it] ); 
      fF1[it]->AddFrame(fTextEntry[it], fLLeft);
      fF1[it]->Resize(fF1[it]->GetDefaultWidth(),pgi->GetPixelY());
      lque = fF1[it]->GetWidth();
      
      if (maxF1[colu] < lque) maxF1[colu] = lque;
      fCol[colu]->AddFrame(fF1[it],fLLeft);
      
      break;
      
    case 2: // combo box
      fF1[it] = new TGCompositeFrame(fCol[colu],  100, pgi->GetPixelY(), 
				     kHorizontalFrame | kFixedHeight);
      fF1[it]->SetBackgroundColor(pgi->GetBgColor());
      
      // add space
      fSpace[it] = new TGCompositeFrame(fF1[it], 0, 20,
					kFixedHeight | kFixedWidth);
      fF1[it]->AddFrame(fSpace[it], fLLeft);
      
      // add question
      fLabel[it] = new TGLabel(fF1[it], new TGString(pgi->fQuestion[it]), 
			       fTextGC(), labelfont);
      fLabel[it]->SetBackgroundColor(pgi->GetBgColor());
      fLabel[it]->Resize(fLabel[it]->GetDefaultWidth(),
			 fLabel[it]->GetDefaultHeight());
      fF1[it]->AddFrame(fLabel[it], fLLeft);
      lque = fLabel[it]->GetWidth();
      fF1[it]->Resize(fF1[it]->GetDefaultWidth(),pgi->GetPixelY());
      lque = fF1[it]->GetWidth();
      if (maxlabel[colu] < lque) maxlabel[colu] = lque;
      
      // add combo
      fCombo[it] = new TGComboBox(fF1[it], it);
      nentries=0;
      nentries = pgi->fComboNoEntries[it];
      //printf("PGInputWindow: it=%d - nentries=%d\n",it,nentries);
      if (nentries>MAXENTRIES) nentries=MAXENTRIES;
      pgi->fComboNoEntries[it] = nentries;
      for (j=0; j < nentries; j++) {
	//printf("PGInputWindow: entry[%d] = %s\n",j,pgi->fEntryList[it][j].Data());
      	fCombo[it]->AddEntry(pgi->fEntryList[it][j], j);
      }
      fCombo[it]->Select(pgi->fComboSelection[it]); // default selection
      l = pgi->fLenght[it]*pgi->GetPixelX()*4/6 + 32;
      fCombo[it]->Resize(l, 20);
      fCombo[it]->Associate(this);
      fF1[it]->AddFrame(fCombo[it], fLLeft);
      fF1[it]->Resize(fF1[it]->GetDefaultWidth(),pgi->GetPixelY());
      lque = fF1[it]->GetWidth();
      if (maxF1[colu] < lque) maxF1[colu] = lque;
      
      fCol[colu]->AddFrame(fF1[it],fLLeft);
      break;
      
    case 3: // check button
      fF1[it] = new TGCompositeFrame(fCol[colu],  100, pgi->GetPixelY(), 
				     kHorizontalFrame | kFixedHeight);
      fF1[it]->SetBackgroundColor(pgi->GetBgColor());
      
      // add space
      fSpace[it] = new TGCompositeFrame(fF1[it], 0, 20,
					kFixedHeight | kFixedWidth);
      fF1[it]->AddFrame(fSpace[it], fLLeft);
      
      // add question
      fLabel[it] = new TGLabel(fF1[it], new TGString(pgi->fQuestion[it]), 
			       fTextGC(), labelfont);
      fLabel[it]->SetBackgroundColor(pgi->GetBgColor());
      fLabel[it]->Resize(fLabel[it]->GetDefaultWidth(),
			 fLabel[it]->GetDefaultHeight());
      fF1[it]->AddFrame(fLabel[it], fLLeft);
      lque = fLabel[it]->GetWidth();
      fF1[it]->Resize(fF1[it]->GetDefaultWidth(),pgi->GetPixelY());
      lque = fF1[it]->GetWidth();
      if (maxlabel[colu] < lque) maxlabel[colu] = lque;
      
      j = atoi(pgi->fFileType[it]);
      fCheck[it] = new TGCheckButton(fF1[it], "", it+10);
      fCheck[it]->SetState((EButtonState)j);
      fCheck[it]->SetBackgroundColor(pgi->GetBgColor());
      if (pgi->fComboSelection[it])
	fCheck[it]->Associate(this); // questo solo se autoupdata...
      fF1[it]->AddFrame(fCheck[it], fLLeft);
      fF1[it]->Resize(fF1[it]->GetDefaultWidth(),pgi->GetPixelY());
      lque = fF1[it]->GetWidth();
      if (maxF1[colu] < lque) maxF1[colu] = lque;
      fCol[colu]->AddFrame(fF1[it],fLLeft);
      break;
      
    case 4: // openfile
      fF1[it] = new TGCompositeFrame(fCol[colu],  100, pgi->GetPixelY(), 
				     kHorizontalFrame | kFixedHeight);
      fF1[it]->SetBackgroundColor(pgi->GetBgColor());
      
      // add space
      fSpace[it] = new TGCompositeFrame(fF1[it], 0, 20,
					kFixedHeight | kFixedWidth);
      fF1[it]->AddFrame(fSpace[it], fLLeft);
      
      // add question
      fLabel[it] = new TGLabel(fF1[it], new TGString(pgi->fQuestion[it]), 
			       fTextGC(), labelfont);
      fLabel[it]->SetBackgroundColor(pgi->GetBgColor());
      fLabel[it]->Resize(fLabel[it]->GetDefaultWidth(),
			 fLabel[it]->GetDefaultHeight());
      fF1[it]->AddFrame(fLabel[it], fLLeft);
      lque = fLabel[it]->GetWidth();
      fF1[it]->Resize(fF1[it]->GetDefaultWidth(),pgi->GetPixelY());
      lque = fF1[it]->GetWidth();
      if (maxlabel[colu] < lque) maxlabel[colu] = lque;
      
      // add answer field and button
      fTbuf[it] = new TGTextBuffer(100); // dimensione buffer
      fTextEntry[it] = new TGTextEntry(fF1[it], fTbuf[it]);
      l = (pgi->fLenght[it]-4)*pgi->GetPixelX();
      fTextEntry[it]->Resize(l, fTextEntry[it]->GetDefaultHeight());
      fTbuf[it]->AddText(0, pgi->fAnswer[it] ); 
      fF1[it]->AddFrame(fTextEntry[it], fLLeft);
      fButton[it] =  new TGTextButton(fF1[it], "Browse", it+10);
      fButton[it]->Associate(this);
      fButton[it]->ChangeBackground(pgi->GetButtonColor());
      fF1[it]->AddFrame(fButton[it],fLLeft);
      
      fF1[it]->Resize(fF1[it]->GetDefaultWidth(),pgi->GetPixelY());
      lque = fF1[it]->GetWidth();
      if (maxF1[colu] < lque) maxF1[colu] = lque;
      fCol[colu]->AddFrame(fF1[it],fLLeft);
      break;
      
    case 5: // button
      fF1[it] = new TGCompositeFrame(fCol[colu],  100, pgi->GetPixelY(), 
				     kHorizontalFrame | kFixedHeight);
      fF1[it]->SetBackgroundColor(pgi->GetBgColor());
      
      fButton[it] =  new TGTextButton(fF1[it], pgi->fQuestion[it], it+10);
      fButton[it]->Associate(this);
      fButton[it]->ChangeBackground(pgi->GetButtonColor());
      fF1[it]->AddFrame(fButton[it],fLLeft);
      fF1[it]->Resize(fF1[it]->GetDefaultWidth(),pgi->GetPixelY());
      lque = fF1[it]->GetWidth();
      if (maxF1[colu] < lque) maxF1[colu] = lque;
      fCol[colu]->AddFrame(fF1[it],fLCenter);
      //printf("PGInputWindow: add button:  it=%d  -  pointer=0x%x\n",it,fButton[it]);

      break;
      
    case 6: // text comment
      fF1[it] = new TGCompositeFrame(fCol[colu],  100, pgi->GetPixelY(), 
      			  kHorizontalFrame | kFixedHeight);
      fF1[it]->SetBackgroundColor(pgi->GetBgColor());
      // add text
      fLabel[it] = new TGLabel(fF1[it], new TGString(pgi->fQuestion[it]), 
			       fCommentGC(), labelfont);
      fLabel[it]->SetBackgroundColor(pgi->GetBgColor());
      fLabel[it]->Resize(fLabel[it]->GetDefaultWidth(),
			 fLabel[it]->GetDefaultHeight());
      fF1[it]->AddFrame(fLabel[it], fLLeft);
      lque = fLabel[it]->GetWidth();
      fF1[it]->Resize(fF1[it]->GetDefaultWidth(),pgi->GetPixelY());
      lque = fF1[it]->GetWidth();
      if (maxlabel[colu] < lque) maxlabel[colu] = lque;
      if (maxF1[colu] < lque) maxF1[colu] = lque;

      if (!strcmp(pgi->fAnswer[it],"l"))
	fCol[colu]->AddFrame(fF1[it],fLLeft);
      else if (!strcmp(pgi->fAnswer[it],"r"))
	fCol[colu]->AddFrame(fF1[it],fLRight);
      else
	fCol[colu]->AddFrame(fF1[it],fLCenter);

      break;

    case 7: // info line
      fF1[it] = new TGCompositeFrame(fCol[colu],  700, pgi->GetPixelY(), 
				     kHorizontalFrame | kFixedHeight);
      
      fF1[it]->SetBackgroundColor(pgi->GetBgColor());
      // add space of fLenght[] points
      fSpace[it] = new TGCompositeFrame(fF1[it], pgi->fLenght[it], 20,
					kFixedHeight | kFixedWidth);
      fSpace[it]->SetBackgroundColor(pgi->GetBgColor());
      fF1[it]->AddFrame(fSpace[it], fLLeft);      
      // add text
      fLabel[it] = new TGLabel(fF1[it], new TGString(pgi->fQuestion[it]),
			       fInfoGC(), labelfont);
      fLabel[it]->SetBackgroundColor(pgi->GetBgColor());
      if (pgi->fButChoice[it])
	fLabel[it]->Resize(pgi->fButChoice[it],
			   fLabel[it]->GetDefaultHeight());	
      else
	fLabel[it]->Resize(fLabel[it]->GetDefaultWidth(),
			   fLabel[it]->GetDefaultHeight());
      
      fF1[it]->AddFrame(fLabel[it], fLLeft);
      if (pgi->fButChoice[it]) 
	fF1[it]->Resize(pgi->fButChoice[it], pgi->GetPixelY());
      else
	fF1[it]->Resize(fF1[it]->GetDefaultWidth(), pgi->GetPixelY());
      
      lque = fF1[it]->GetWidth();
      if (maxlabel[colu] < lque) maxlabel[colu] = lque;
      if (maxF1[colu] < lque) maxF1[colu] = lque;
      fCol[colu]->AddFrame(fF1[it],fLLeft);
      break;
      
    default:
      break;
    }
  }
  
  // resize frames
  for (Int_t i=0; i<pgi->GetNoItems(); i++) {    
    Int_t wid = pgi->fWidget[i];
    if (wid==0 || wid==5 || wid==6 || wid==7) continue;
    // there is a label and a space
    fSpace[i]->SetBackgroundColor(pgi->GetBgColor());
    fSpace[i]->SetWidth(maxlabel[pgi->fCol[i]] - fLabel[i]->GetWidth());
    fF1[i]->SetWidth(fF1[i]->GetDefaultWidth());
  }
  
  for (Int_t i=0; i<ncol; i++) {
    fCol[i]->Resize(fCol[i]->GetDefaultWidth(),fCol[i]->GetDefaultHeight());
    fCentral->AddFrame(fCol[i],fLCol);
  }  

  fCentral->Resize(fCentral->GetDefaultWidth(),fCentral->GetDefaultHeight());
 
  AddFrame(fCentral,fLMain);

  // frame for OK,APPLY and CANCEL buttons

  fFinal = new TGCompositeFrame(this, 300, 50, kHorizontalFrame);
  fFinal->SetBackgroundColor(pgi->GetBgColor());

  if (pgi->fIsOK) {
    fButtonOK = new TGTextButton(fFinal, "&O.K.  ", 1);
    fButtonOK->Associate(this);
    fButtonOK->ChangeBackground(pgi->GetButtonColor());
    fFinal->AddFrame(fButtonOK, fLNormal);
  }

  if (pgi->fIsCANCEL) {
    fButtonCANCEL = new TGTextButton(fFinal, "&Cancel", 2);
    fButtonCANCEL->Associate(this);
    fButtonCANCEL->ChangeBackground(pgi->GetButtonColor());
    fFinal->AddFrame(fButtonCANCEL, fLNormal);
  }

  if (pgi->fIsAPPLY) {
    fButtonAPPLY = new TGTextButton(fFinal, "&Apply", 3);  
    fButtonAPPLY->Associate(this); 
    fButtonAPPLY->ChangeBackground(pgi->GetButtonColor());  
    fFinal->AddFrame(fButtonAPPLY, fLNormal);
  }

  if (pgi->fIsREFRESH) {
    fButtonREFRESH = new TGTextButton(fFinal, "&Refresh", 4);  
    fButtonREFRESH->Associate(this); 
    fButtonREFRESH->ChangeBackground(pgi->GetButtonColor());  
    fFinal->AddFrame(fButtonREFRESH, fLNormal);
  }

  if (pgi->fIsQUIT) {
    fButtonQUIT = new TGTextButton(fFinal, "&QUIT", 5);  
    fButtonQUIT->Associate(this); 
    fButtonQUIT->ChangeBackground(pgi->GetButtonColor());  
    fFinal->AddFrame(fButtonQUIT, fLNormal);
  }
  
  AddFrame(fFinal, fLCenter);
  SetBackgroundColor(pgi->GetBgColor());
  Resize(GetDefaultWidth(), GetDefaultHeight());

  MapSubwindows();
  
  Layout();
  SetWindowName(pgi->GetTitle());
  SetIconName(pgi->GetTitle());
  
  MapWindow();
  pgi->fIsWin = 1;

  pgi->fWin = this;

  if (pgi->fWait) 
    fClient->WaitFor(this);
}

//______________________________________________________________________________
PGInputWindow::~PGInputWindow()
{
  if (pgi->fIsQUIT) delete fButtonQUIT;
  if (pgi->fIsREFRESH) delete fButtonREFRESH;
  if (pgi->fIsAPPLY) delete fButtonAPPLY;
  if (pgi->fIsCANCEL) delete fButtonCANCEL;
  if (pgi->fIsOK) delete fButtonOK;
  delete fFinal; 
  
  Int_t ni = pgi->GetNoItems();  
  for (Int_t i=(ni-1); i>=0; i--) {
    
    switch (pgi->fWidget[i]) {
      
    case 1: // edit box
      delete fTextEntry[i];
      delete fLabel[i];
      delete fSpace[i];
      delete fF1[i];
      break;

    case 2: // combo box
      delete fCombo[i];
      delete fLabel[i];
      delete fSpace[i];
      delete fF1[i];
      break;
      
    case 3:
      delete fCheck[i];
      delete fLabel[i];
      delete fSpace[i];
      delete fF1[i];
      break;      

    case 4: // openfile
      delete fButton[i];
      delete fTextEntry[i];
      delete fLabel[i];
      delete fSpace[i];
      delete fF1[i];
      break;

    case 5: // button
      delete fButton[i];
      delete fF1[i];
      break;

    case 6: // button
      delete fLabel[i];
      delete fF1[i];
      break;
      
    case 7: // info
      delete fLabel[i];
      delete fSpace[i];
      delete fF1[i];
      break;

    case 8:
      delete fCheck[i];
      delete fLabel[i];
      delete fSpace[i];
      delete fF1[i];
      break; 
      
    default:
      break;      
      
    }
  }
  for (Int_t i=fNcol-1; i>=0; i--) 
    delete fCol[i];
  
  delete fCentral;

  delete fLNormal;
  delete fLCenter;
  delete fLMain;
  delete fLRight;
  delete fLCol;
  delete fLLeft;

  if (pgi) pgi->SetIsWin(0);
  // test for default font restoring... 
  TGTextEntry *tmp=new TGTextEntry(this,"temp");
  //tmp->SetFont(tmp->GetDefaultFontStruct());
  delete tmp;
}
//______________________________________________________________________________
void PGInputWindow::CloseWindow()
{
  if (!pgi->fIsOK && !pgi->fIsCANCEL) return;
  if (pgi) pgi->SetIsWin(0);  
  //TGTransientFrame::CloseWindow();
  //TGMainFrame::UnmapWindow();
  //gApplication->ReturnFromRun();
  //printf("closewindows\n");
  delete this;
}
//______________________________________________________________________________
void PGInputWindow::UpdateData(Int_t idx)
{
  if (idx >= pgi->fNoItems) return;
  Int_t ffrom=idx;
  Int_t fto=idx+1;
  if (idx<0) {
    ffrom=0;
    fto=pgi->GetNoItems();
  }  

  for (Int_t i=ffrom; i<fto; i++) {
    switch (pgi->fWidget[i]) {
    case 1: // edit box
      strcpy(pgi->fAnswer[i], fTbuf[i]->GetString());
      break;

    case 2: // combo box
      pgi->fComboSelection[i] = fCombo[i]->GetSelected();
      break;

    case 3: // check button
      if (fCheck[i]->GetState()) strcpy(pgi->fFileType[i],"1");
      else strcpy(pgi->fFileType[i],"0");
      break;

    case 4: // openfile
      strcpy(pgi->fAnswer[i], fTbuf[i]->GetString());
      break;      

    default:
      break;
    }
  }
}
//______________________________________________________________________________
Bool_t PGInputWindow::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2)
{
  
  // Process events generated by the buttons in the frame.
  Long_t pippol=parm2; pippol++; // ZPE
  //printf("msg=%ld  parm1=%ld   parm2=%ld\n",msg,parm1,parm2);
  
  switch (GET_MSG(msg)) {
  case kC_COMMAND:
    switch (GET_SUBMSG(msg)) {
    case kCM_BUTTON:
      switch(parm1) {

      case 1: // OK
	if (pgi->fLenght[MAXPGLINES+1]==1)
	  ProcessCommand(MAXPGLINES+1);
	pgi->UpdateData();
	if (pgi->fLenght[MAXPGLINES+1]==2)
	  ProcessCommand(MAXPGLINES+1);
	pgi->fRetval=1;

      case 2: // CANCEL
	CloseWindow();
	break;

      case 3: // APPLY
	if (pgi->fLenght[MAXPGLINES+2]==1)
	  ProcessCommand(MAXPGLINES+2);
	pgi->UpdateData();
	if (pgi->fLenght[MAXPGLINES+2]==2)
	  ProcessCommand(MAXPGLINES+2);
	break;

      case 4: // REFRESH
	if (pgi->fLenght[MAXPGLINES+3]==1)
	  ProcessCommand(MAXPGLINES+3);
	pgi->UpdateField();
	if (pgi->fLenght[MAXPGLINES+3]==2)
	  ProcessCommand(MAXPGLINES+3);
	break;
	
      case 5: // QUIT
	if (!pgi->inpYN("QUIT! Are you sure?"))
	  return kTRUE;
	else 
	  gROOT->ProcessLine(".qqq");
	break;

      default:
	ProcessButton(parm1);
	break;
      }
      break;

    case kCM_COMBOBOX:
      ProcessCommand((Int_t)parm1);
      break;

    case kCM_CHECKBUTTON:
      //printf("... beccato il messaggio... Processo %d\n",(Int_t)parm1-10);
      pgi->UpdateData((Int_t)parm1-10);
      ProcessCommand((Int_t)parm1-10);
      break;

    default:
      break;
    }
  default:
    break;
  }
  return kTRUE;
  
}
//______________________________________________________________________________
void PGInputWindow::ProcessButton(Long_t parm1)
{
  Int_t idx = parm1-10;
  //PInput p; // ZPE: tolta la PInput dep.
  Char_t tmp[100];

  switch (pgi->fWidget[idx]) {
  case 2: // combobox
    ProcessCommand(idx);
    break;

  case 4: // openfile
    if (pgi->InputFilename(tmp,pgi->fFileType[idx])) return;  // ZPE: tolta la PInput dep.
    fTbuf[idx]->RemoveText(0,100);
    fTbuf[idx]->AddText(0,tmp);
    fClient->NeedRedraw(fTextEntry[idx]);
    break;

  case 5: // button
    if (pgi->fAddr[idx]==1) pgi->UpdateData();
    if (pgi->fAddr[idx]==2) pgi->UpdateField();
    ProcessCommand(idx);
    break;

  default:
    break;
  }
}
//______________________________________________________________________________
void PGInputWindow::ProcessCommand(Int_t idx)
{
  if (!strcmp(pgi->fAnswer[idx],"")) return;
  Int_t usub=0;
  if (strstr(pgi->fAnswer[idx],"usersub")) {
    usub = pgi->fAnswer[idx][7] - 48;

    if (!usersubptr) return;

    switch (usub) {
    case 1:
      usersubptr->Usersub1(pgi->fButChoice[idx], pgi->fButPar[idx].GetArray());
      break;
    case 2:
      usersubptr->Usersub2(pgi->fButChoice[idx], pgi->fButPar[idx].GetArray());
      break;
    case 3:
      usersubptr->Usersub3(pgi->fButChoice[idx], pgi->fButPar[idx].GetArray());
      break;
    case 4:
      usersubptr->Usersub4(pgi->fButChoice[idx], pgi->fButPar[idx].GetArray());
      break;
    case 5:
      usersubptr->Usersub5(pgi->fButChoice[idx], pgi->fButPar[idx].GetArray());
      break;
    case 6:
      usersubptr->Usersub6(pgi->fButChoice[idx], pgi->fButPar[idx].GetArray());
      break;
    case 7:
      usersubptr->Usersub7(pgi->fButChoice[idx], pgi->fButPar[idx].GetArray());
      break;
    case 8:
      usersubptr->Usersub8(pgi->fButChoice[idx], pgi->fButPar[idx].GetArray());
      break;
    case 9:
      usersubptr->Usersub9(pgi->fButChoice[idx], pgi->fButPar[idx].GetArray());
      break;
    }
  }
  else 
    gROOT->ProcessLine(pgi->fAnswer[idx]);  
}
//______________________________________________________________________________
void PGInputWindow::UpdateField(Int_t idx)
{
  if (idx >= pgi->fNoItems) return;
  Int_t ffrom=idx;
  Int_t fto=idx+1;
  if (idx<0) {
    ffrom=0;
    fto=pgi->GetNoItems();
  }  

  for (Int_t i=ffrom; i<fto; i++) {

    switch (pgi->fWidget[i]) {
    case 1:
      fTbuf[i]->RemoveText(0,100);
      fTbuf[i]->AddText(0, pgi->fAnswer[i]);
      fClient->NeedRedraw(fTextEntry[i]);
      break;

    case 2:
      fCombo[i]->Select( pgi->fComboSelection[i] );
      break;

    case 3:
      int j;
      j = atoi(pgi->fFileType[i]);
      fCheck[i]->SetState((EButtonState)j);
      break;

    case 4:
      fTbuf[i]->RemoveText(0,100);
      fTbuf[i]->AddText(0, pgi->fAnswer[i]);
      fClient->NeedRedraw(fTextEntry[i]);
      break;

    case 7:
      fLabel[i]->SetText(new TGString(pgi->fQuestion[i]));
      fF1[i]->Resize(fF1[i]->GetDefaultSize());
      break;


    default:
      break;
    }
    //printf("PGInputWindow::UpdateField() - uscito dallo switch...\n"); // zpe 
  }
}

//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
PGInput::PGInput()
{
  fWin = NULL;
  fIsWin = 0;
  fSizeX = 10;
  fSizeY = 10;
  fPx = 12;
  fPy = 24;

  strcpy(fCommentColor,"black");
  //strcpy(fFgColor,"darkblue");
  strcpy(fFgColor,"blue");
  strcpy(fBgColor,"grey80");
  strcpy(fButtonColor,"grey80");
  //strcpy(fInfoColor,"red3");
  strcpy(fInfoColor,"red");
  strcpy(fTitle,"Input");
  fLenghtQ = 10;
  fLenghtA = 10;
  Reset();
}
//______________________________________________________________________________
void PGInput::Reset()
{
  Close();
  fNoItems = 0;
  fNoInfo = 0;
  fParente=NULL;
  fRetval = 0;
  fWait = 0;
  for (Int_t i=0; i<MAXPGLINES; i++) {
    strcpy(fQuestion[i],"");
    fAddr[i] = 0;
    fType[i] = ' ';
    fCol[i] = 0;
    fWidget[i] = 0;
    fEntryList[i] = NULL;
    strcpy(fFileType[i],"");
    fComboSelection[i] = 0;
    fComboNoEntries[i] = 0;
    fInfoLine[i]=0;
  }
  for (Int_t i=0; i<MAXPGLINES+5; i++) {
    strcpy(fAnswer[i],"");
    fLenght[i] = 0;
    fButChoice[i] = 0;
    fButPar[i].Set(20); // max 20 params
    fButPar[i].Reset();
  }
  fontsel=0;
  fIsOK = 0;
  fIsAPPLY = 0;
  fIsCANCEL = 0;
  fIsREFRESH = 0;
  fIsQUIT = 0;
}
//______________________________________________________________________________
PGInput::~PGInput()
{
  if (fIsWin && fWin) delete fWin;
}
//______________________________________________________________________________
Int_t PGInput::AddEditBox(const Char_t *question, Char_t *string, 
			  Int_t lenght, Int_t col)
{
  // Add a string edit box:
  // string  is the address of the string to be edited
  // lenght  is the lenght of the input field in char [strlen(string)]
  // col     is the column number [1]
  // return  widget number if success, <0 if error
  if (strlen(question)>79) {
    printf("too long question!\n");
    return -1;
  }
  if (col<1 || col>10) return -1;

  strcpy(fQuestion[fNoItems],question);
  strcpy(fAnswer[fNoItems],string);

  if (!lenght) fLenght[fNoItems]=strlen(string);
  else fLenght[fNoItems]=lenght;

  fType[fNoItems] = 's';
  fWidget[fNoItems] = 1;
  fAddr[fNoItems] = (Long_t) string;
  fCol[fNoItems] = col-1;
  fNoItems++;
  return (fNoItems-1);
}
//______________________________________________________________________________
Int_t PGInput::AddEditBox(const Char_t *question, Int_t& num, 
			  Int_t lenght, Int_t col)
{
  // As the string edit box for integer
  // return  widget number if success, <0 if error
  if (strlen(question)>79) {
    printf("too long question!\n");
    return -1;
  }
  if (col<1 || col>10) return -1;
  strcpy(fQuestion[fNoItems],question);
  sprintf(fAnswer[fNoItems],"%d",num);
  if (!lenght) fLenght[fNoItems]=strlen(fAnswer[fNoItems]);
  else fLenght[fNoItems]=lenght;
  fType[fNoItems] = 'i';
  fWidget[fNoItems] = 1;
  fAddr[fNoItems] = (Long_t) &num;
  fCol[fNoItems] = col-1;
  fNoItems++;
  return (fNoItems-1);
}
//______________________________________________________________________________
Int_t PGInput::AddEditBox(const Char_t *question, Long_t& num, 
			  Int_t lenght, Int_t col)
{
  // As the string edit box for long
  // return  widget number if success, <0 if error
  if (strlen(question)>79) {
    printf("too long question!\n");
    return -1;
  }
  if (col<1 || col>10) return -1;
  strcpy(fQuestion[fNoItems],question);
  sprintf(fAnswer[fNoItems],"%ld",num);
  if (!lenght) fLenght[fNoItems]=strlen(fAnswer[fNoItems]);
  else fLenght[fNoItems]=lenght;
  fType[fNoItems] = 'l';
  fWidget[fNoItems] = 1;
  fAddr[fNoItems] = (Long_t) &num;
  fCol[fNoItems] = col-1;
  fNoItems++;
  return (fNoItems-1);
}
//______________________________________________________________________________
Int_t PGInput::AddEditBox(const Char_t *question, Short_t& num, 
			  Int_t lenght, Int_t col)
{
  // As the string edit box for short
  // return  widget number if success, <0 if error
  if (strlen(question)>79) {
    printf("too long question!\n");
    return -1;
  }
  if (col<1 || col>10) return -1;
  strcpy(fQuestion[fNoItems],question);
  sprintf(fAnswer[fNoItems],"%hd",num);
  if (!lenght) fLenght[fNoItems]=strlen(fAnswer[fNoItems]);
  else fLenght[fNoItems]=lenght;
  fType[fNoItems] = 'h';
  fWidget[fNoItems] = 1;
  fAddr[fNoItems] = (Long_t) &num;
  fCol[fNoItems] = col-1;
  fNoItems++;
  return (fNoItems-1);
}
//______________________________________________________________________________
Int_t PGInput::AddEditBox(const Char_t *question, Float_t& num, 
			  Int_t lenght, Int_t col)
{
  // As the string edit box for float
  // return  widget number if success, <0 if error
  if (strlen(question)>79) {
    printf("too long question!\n");
    return -1;
  }
  if (col<1 || col>10) return -1;
  strcpy(fQuestion[fNoItems],question);
  sprintf(fAnswer[fNoItems],"%g",num);
  if (!lenght) fLenght[fNoItems]=strlen(fAnswer[fNoItems]);
  else fLenght[fNoItems]=lenght;
  fType[fNoItems] = 'f';
  fWidget[fNoItems] = 1;
  fAddr[fNoItems] = (Long_t) &num;
  fCol[fNoItems] = col-1;
  fNoItems++;
  return (fNoItems-1);
}
//______________________________________________________________________________
Int_t PGInput::AddEditBox(const Char_t *question, Double_t& num, 
			  Int_t lenght, Int_t col)
{
  // As the string edit box for double
  // return  widget number if success, <0 if error
  if (strlen(question)>79) {
    printf("too long question!\n");
    return -1;
  }
  if (col<1 || col>10) return -1;
  strcpy(fQuestion[fNoItems],question);
  sprintf(fAnswer[fNoItems],"%g",num);
  if (!lenght) fLenght[fNoItems]=strlen(fAnswer[fNoItems]);
  else fLenght[fNoItems]=lenght;
  fType[fNoItems] = 'd';
  fWidget[fNoItems] = 1;
  fAddr[fNoItems] = (Long_t) &num;
  fCol[fNoItems] = col-1;
  fNoItems++;
  return (fNoItems-1);
}
//______________________________________________________________________________
Int_t PGInput::AddEditBox(const Char_t *question, Char_t& c, Int_t col)
{
  // As the string edit box for double
  // return  widget number if success, <0 if error
  if (strlen(question)>79) {
    printf("too long question!\n");
    return -1;
  }
  if (col<1 || col>10) return -1;
  strcpy(fQuestion[fNoItems],question);
  sprintf(fAnswer[fNoItems],"%c",c);
  fLenght[fNoItems] = 2;
  fType[fNoItems] = 'c';
  fWidget[fNoItems] = 1;
  fAddr[fNoItems] = (Long_t) &c;
  fCol[fNoItems] = col-1;
  fNoItems++;
  return (fNoItems-1);
}
//______________________________________________________________________________
//______________________________________________________________________________
Int_t PGInput::AddEditBox(const Char_t *question, Bool_t& yn, Int_t col, int autoupdate, const Char_t *command, Int_t choice)
{
  // Add a check entry for boolean
  // return  widget number if success, <0 if error
  // if autoupdate=1 the yn value is immediately set at press
  if (strlen(question)>79) {
    printf("too long question!\n");
    return -1;
  }
  if (col<1 || col>10) return -1;
  strcpy(fQuestion[fNoItems],question);
  sprintf(fFileType[fNoItems],"%d",yn);
  fLenght[fNoItems]=3;
  fType[fNoItems] = 'b';
  fWidget[fNoItems] = 3;
  fAddr[fNoItems] = (Long_t) &yn;
  fCol[fNoItems] = col-1;
  fButChoice[fNoItems] = choice;
  if (autoupdate) fComboSelection[fNoItems]=1;
  else fComboSelection[fNoItems]=0;
  if (command) {strcpy(fAnswer[fNoItems],command);fComboSelection[fNoItems]=1;}
  else strcpy(fAnswer[fNoItems],""); 
  fNoItems++;
  return (fNoItems-1);
}
//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
Int_t PGInput::AddComboBox(const Char_t *question, TGString *entries, 
			   Int_t& selection, const Char_t *command, Int_t choice,
			   Long_t *par, Int_t col)
{
  // Add a combo box with default selection
  // entries    is the array with the selection entries (last = null)
  // selection  is the default selection
  // command    is the line command to be executed when changing 
  //            selection [NULL]
  // choice/par if command="usersubN" the PGInputUsersubN(choice,par)
  //            is executed [0/NULL]
  // col        is the column number [1]
  // return     widget number if success, <0 if error
  if (strlen(question)>79) {
    printf("too long question!\n");
    return -1;
  }
  if (col<1 || col>10) return -1;
  Int_t nentries=0;
  Int_t maxl=0;
  while (entries[nentries] &&
	 entries[nentries].CompareTo("EOE") ) {
    if ( (int)entries[nentries].Length() > maxl) 
      maxl = entries[nentries].Length();
    //printf("PGInput::AddComboBox: got entry %d - %s\n",nentries,entries[nentries].Data());
    nentries++;
  }
  if (nentries>MAXENTRIES) {
    printf("too many entries!\n");
    return -2;
  }
  if (selection<0 || selection>=nentries) {
    printf("wrong default selection!\n");
    return -3;
  }
  fComboNoEntries[fNoItems] = nentries;

  fEntryList[fNoItems] = new TGString[nentries];
  for (int ij=0; ij<fComboNoEntries[fNoItems]; ij++){
    fEntryList[fNoItems][ij]=entries[ij];
    //printf("PGInput::AddComboBox: NoItem=%d - store entry %d - %s\n",fNoItems,ij,fEntryList[fNoItems][ij].Data());
  }

  strcpy(fQuestion[fNoItems],question);
  fComboSelection[fNoItems] = selection;
  if (command) strcpy(fAnswer[fNoItems],command);
  else strcpy(fAnswer[fNoItems],""); 

  fLenght[fNoItems]=maxl;
  fType[fNoItems] = 'C';
  fWidget[fNoItems] = 2;
  fButChoice[fNoItems] = choice;
  fButPar[fNoItems].Set(20,par);
  fAddr[fNoItems] = (Long_t) &selection;
  fCol[fNoItems] = col-1;
  fNoItems++;
  return (fNoItems-1);
}
//______________________________________________________________________________
Int_t PGInput::AddOpenFile(const Char_t *question, const Char_t *filename, const Char_t *defext, 
			   Int_t lenght, Int_t col)
{
  // As the string edit box for filname with Broesw button
  // defext is the default file extension [root]
  // return  widget number if success, <0 if error
  if (strlen(question)>79) {
    printf("too long question!\n");
    return -1;
  }
  if (col<1 || col>10) return -1;
  strcpy(fQuestion[fNoItems],question);
  strcpy(fAnswer[fNoItems],filename);
  strcpy(fFileType[fNoItems],defext);
  if (!lenght) fLenght[fNoItems]=strlen(filename);
  else fLenght[fNoItems]=lenght;
  fLenght[fNoItems] += 4;
  fType[fNoItems] = 'F';
  fWidget[fNoItems] = 4;
  fAddr[fNoItems] = (Long_t) filename;
  fCol[fNoItems] = col-1;
  fNoItems++;
  return (fNoItems-1);
}
//______________________________________________________________________________
Int_t PGInput::AddButton(const Char_t *question, const Char_t *command, Int_t choice,
			 Long_t *par, Int_t col, Int_t before)
{
  // Add a button
  // question   is the text in the button
  // command    is the line command to be executed when changing 
  //            selection [NULL]
  // choice/par if command="usersubN" the PGInputUsersubN(choice,par)
  //            is executed [0/NULL]
  // col        is the column number [1]
  // before     update data(1), fields(2), none(0) before executing command [0]
  // return     widget number if success, <0 if error
  if (strlen(question)>79) {
    printf("too long question!\n");
    return -1;
  }
  if (col<1 || col>10) return -1;
  strcpy(fQuestion[fNoItems],question);
  if (command) strcpy(fAnswer[fNoItems],command);
  else strcpy(fAnswer[fNoItems],""); 
  fLenght[fNoItems]=strlen(question);
  fType[fNoItems] = 'B';
  fButChoice[fNoItems] = choice;
  fWidget[fNoItems] = 5;
  fAddr[fNoItems] = (Long_t) before;
  fButPar[fNoItems].Set(20,par);
  //fButPar[fNoItems] = par;
  fCol[fNoItems] = col-1;
  fNoItems++;
  return (fNoItems-1);
}
//______________________________________________________________________________
Int_t PGInput::AddComment(const Char_t *comment, Int_t col, const Char_t *layout)
{
  // Add a comment in the column col
  // return  widget number if success, <0 if error
  // layout = (c)enter, (r)ight or (l)left

  if (strlen(comment)>79) {
    printf("too long comment!\n");
    return -1;
  }
  if (col<1 || col>10) return -1;
  if (!strstr("crl",layout)) return -1;
  strcpy(fQuestion[fNoItems],comment);
  strcpy(fAnswer[fNoItems],layout);
  fLenght[fNoItems]=strlen(comment);
  fType[fNoItems] = 't'; // text
  fWidget[fNoItems] = 6;
  fAddr[fNoItems] = 0;
  fCol[fNoItems] = col-1;
  fNoItems++;
  return (fNoItems-1);
}
//______________________________________________________________________________
Int_t PGInput::AddInfo(const Char_t *info, Int_t shift, Int_t col, Int_t len)
{  
  // Add a line for information
  // Use SendInfo() to send information when window is open
  // shift = number of points from left edge

  if (strlen(info)>79) {
    printf("too long info!\n");
    return -1;
  }
  if (col<1 || col>10) return -1;
  strcpy(fQuestion[fNoItems],info);
  strcpy(fAnswer[fNoItems],"");
  fLenght[fNoItems]=shift;
  fType[fNoItems] = 'I'; // text
  fWidget[fNoItems] = 7;
  fAddr[fNoItems] = 0;
  fCol[fNoItems] = col-1;
  fInfoLine[fNoInfo] = fNoItems;
  fButChoice[fNoItems] = len; // lenght in pixel (optional)
  fNoInfo++;
  fNoItems++;
  return (fNoItems-1);
}
//______________________________________________________________________________
Int_t PGInput::Show(EPGInputButtonType b, Bool_t waitfor)
{
  // Show the input window with buttons OK,CANCEL,APPLY,REFRESH 
  // and QUIT according with b
  //
  // If waitfor=TRUE -> wait for the window tom be destroyed (doesn't work
  //                    properly with TRint)
  // Return value:      -1 if error
  //                     0 if waitfor=FALSE or waitfor=TRUE and exit with CANCEL
  //                     1 if waitfor=TRUE and exit with OK

  if (fIsWin) {
    printf("window already open!\n");
    return 0;
  }
  fWait = waitfor;
  SetButtons(b);
  CalculateSize();
  fRetval=0;

  // // combo test
  // printf("PGInput::Show: this window has %d items\n",fNoItems);
  // for (int it=0; it<fNoItems; it++) { 
  //   if (fWidget[it]==2) {
  //     int nentries=fComboNoEntries[it];
  //     printf("PGInput::Show: found combo item=%d with %d entries\n",it,nentries);
  //     for (int j=0; j < nentries; j++) {
  //      	printf("PGInput::Show: entry[%d] = %s\n",j,fEntryList[it][j].Data());
  //     }  
  //   }
  // }
  // printf("PGInput::Show: son qui!\n");
  // // endtest
  
 
  if (fParente) {
    TGWindow *tmp = (TGWindow *) gClient->GetRoot();
    if (fParente->IsWin()) tmp = fParente->GetPGWindow();
    fWin = new PGInputWindow(gClient->GetRoot(), tmp,
			     fSizeX, fSizeY, this, fUsersubPtr);
  }
  else
    fWin = new PGInputWindow(gClient->GetRoot(), gClient->GetRoot(),
			     fSizeX, fSizeY, this, fUsersubPtr);
  if (!fWin) return -1;
  if (!waitfor) fIsWin = 1;

  //fWin->usersubptr = fUsersubPtr;
  
  return fRetval;
}
//______________________________________________________________________________
void PGInput::UpdateData(Int_t idx)
{
  // Set data as in the window for widget idx
  // idx = -1 means all widgets [-1]

  if (!fIsWin) return;
  Int_t ffrom=idx;
  Int_t fto=idx+1;
  if (idx<0) {
    ffrom=0;
    fto=fNoItems;
  }  

  fWin->UpdateData(idx);
  
  for (Int_t i=ffrom; i<fto; i++) {
    switch (fType[i]) {
      
    case 's': // string
      strcpy((Char_t*)fAddr[i], fAnswer[i]);
      break;
      
    case 'i': // integer
      *((Int_t*)fAddr[i]) = (Int_t)strtod(fAnswer[i], NULL);
      break;

    case 'f': // float
      *((Float_t*)fAddr[i]) = (Float_t)strtod(fAnswer[i], NULL);
      break;

    case 'd': // double
      *((Double_t*)fAddr[i]) = strtod(fAnswer[i], NULL);
      break;

    case 'l': // long
      *((Long_t*)fAddr[i]) = strtod(fAnswer[i], NULL);
      break;

    case 'h': // short
      *((Short_t*)fAddr[i]) = strtod(fAnswer[i], NULL);
      break;

    case 'c': // char
      *((Char_t*)fAddr[i]) = fAnswer[i][0];
      break;

    case 'C': // Combo box
      *((Int_t*)fAddr[i]) = fComboSelection[i];
      break;

    case 'b': // check button
      *((Bool_t*)fAddr[i]) = atoi(fFileType[i]);
      break;

    case 'F': // openfile
      strcpy((Char_t*)fAddr[i], fAnswer[i]);
      break;

    default:
      break;
    }
  }
}
//______________________________________________________________________________
void PGInput::CalculateSize()
{
  Int_t lq=1;
  Int_t la=1;
  Int_t ly=1;

  for (Int_t i=0; i<fNoItems; i++) {
    if (fWidget[i] != 5) 
      if ((int)strlen(fQuestion[i])>lq) lq = strlen(fQuestion[i]);
    if (fLenght[i]>la) la = fLenght[i];          
    ly++;
  }
  fLenghtQ = (lq+1)*fPx + 5;
  fLenghtA = la*fPx/3 + 5;
  fSizeX = fLenghtA + fLenghtQ;
  fSizeY = ly*fPy*4/3 + 10;
  Int_t lmin=0;
  if (fIsOK) lmin++;
  if (fIsAPPLY) lmin++;
  if (fIsCANCEL) lmin++;
  if (fIsREFRESH) lmin++;
  lmin *= 55;
  if (fSizeX < lmin) fSizeX=lmin;
}
//______________________________________________________________________________
void PGInput::UpdateField(Int_t idx)
{
  // Set fields in the window according to present data for widget idx
  // idx = -1 means all widgets [-1]

  if (!fIsWin) return;
  if (idx>=fNoItems) return;

  Int_t ffrom=idx;
  Int_t fto=idx+1;
  if (idx<0) {
    ffrom=0;
    fto=GetNoItems();
  }  
  for (Int_t i=ffrom; i<fto; i++) {
    switch (fType[i]) {

    case 's': // string
      strcpy(fAnswer[i], (Char_t*)fAddr[i]);
      break;

    case 'i': // integer
      sprintf(fAnswer[i],"%d",*((Int_t*)fAddr[i]));
      break;

    case 'f': // float
      sprintf(fAnswer[i],"%g",*((Float_t*)fAddr[i]));
      break;


    case 'd': // double
      sprintf(fAnswer[i],"%g",*((Double_t*)fAddr[i]));
      break;

    case 'l': // long
      sprintf(fAnswer[i],"%ld",*((Long_t*)fAddr[i]));
      break;

    case 'h': // short
      sprintf(fAnswer[i],"%hd",*((Short_t*)fAddr[i]));
      break;

    case 'c': // char
      sprintf(fAnswer[i],"%c",*((Char_t*)fAddr[i]));
      break;

    case 'C': // Combo box
      fComboSelection[i] = *((Int_t*)fAddr[i]);
      break;

    case 'b': // check button
      sprintf(fFileType[i],"%d",*((Bool_t*)fAddr[i]));
      break;

    case 'F': // string
      strcpy(fAnswer[i], (Char_t*)fAddr[i]);
      break;

    default:
      break;
    }
  }

  fWin->UpdateField();
}
//______________________________________________________________________________
void PGInput::SetFgColor(Char_t *colorname)
{
  ULong_t tmp;
  if (!gClient->GetColorByName(colorname,tmp)) return;
  strcpy(fFgColor,colorname);
}
//______________________________________________________________________________
ULong_t PGInput::GetFgColor()
{
  ULong_t tmp = 0; // black
  gClient->GetColorByName(fFgColor,tmp);
  return tmp;
}
//______________________________________________________________________________
void PGInput::SetBgColor(Char_t *colorname)
{
  ULong_t tmp;
  if (!gClient->GetColorByName(colorname,tmp)) return;
  strcpy(fBgColor,colorname);
}
//______________________________________________________________________________
ULong_t PGInput::GetBgColor()
{
  ULong_t tmp = 13421772; // grey80
  gClient->GetColorByName(fBgColor,tmp);
  return tmp;
}
//______________________________________________________________________________
void PGInput::SetButtonColor(const Char_t *colorname)
{
  ULong_t tmp;
  if (!gClient->GetColorByName(colorname,tmp)) return;
  strcpy(fButtonColor,colorname);
}
//______________________________________________________________________________
ULong_t PGInput::GetButtonColor()
{
  ULong_t tmp = 13421772; // grey80
  gClient->GetColorByName(fButtonColor,tmp);
  return tmp;
}
//______________________________________________________________________________
void PGInput::SetInfoColor(const Char_t *colorname)
{
  ULong_t tmp;
  if (!gClient->GetColorByName(colorname,tmp)) return;
  strcpy(fInfoColor,colorname);
}
//______________________________________________________________________________
ULong_t PGInput::GetInfoColor()
{
  ULong_t tmp = 1; // red
  gClient->GetColorByName(fInfoColor,tmp);
  return tmp;
}
//______________________________________________________________________________
void PGInput::SetCommentColor(const Char_t *colorname)
{
  ULong_t tmp;
  if (!gClient->GetColorByName(colorname,tmp)) return;
  strcpy(fCommentColor,colorname);
}
//______________________________________________________________________________
ULong_t PGInput::GetCommentColor()
{
  ULong_t tmp = 0; // black
  gClient->GetColorByName(fCommentColor,tmp);
  return tmp;
}
//______________________________________________________________________________
void PGInput::SetOkAction(const Char_t *command, Int_t choice, 
			  Long_t *par, Int_t when)
{
  // execute command just before(1) or after(2) applying OK

  if (command) strcpy(fAnswer[MAXPGLINES+1],command); // line MAXPGLINES+1 
  else strcpy(fAnswer[MAXPGLINES+1],""); 
  fButChoice[MAXPGLINES+1] = choice;
  fButPar[MAXPGLINES+1].Set(20,par);
  fLenght[MAXPGLINES+1] = when; // means: there is something to do
}
//______________________________________________________________________________
void PGInput::SetApplyAction(const Char_t *command, Int_t choice, 
			     Long_t *par, Int_t when)
{
  // execute command just before(1) or after(2) applying APPLY

  if (command) strcpy(fAnswer[MAXPGLINES+2],command); // line MAXPGLINES+2 
  else strcpy(fAnswer[MAXPGLINES+2],""); 
  fButChoice[MAXPGLINES+2] = choice;
  fButPar[MAXPGLINES+2].Set(20,par);
  fLenght[MAXPGLINES+2] = when; // means: there is something to do
}
//______________________________________________________________________________
void PGInput::SetRefreshAction(const Char_t *command, Int_t choice, 
			       Long_t *par, Int_t when)
{
  // execute command just before(1) or after(2) applying REFRESH
  if (command) strcpy(fAnswer[MAXPGLINES+3],command); // line MAXPGLINES+3 
  else strcpy(fAnswer[MAXPGLINES+3],""); 
  fButChoice[MAXPGLINES+3] = choice;
  fButPar[MAXPGLINES+3].Set(20,par);
  fLenght[MAXPGLINES+3] = when; // means: there is something to do
}
//______________________________________________________________________________
void PGInput::SetButtons(EPGInputButtonType bt) 
{
  Int_t b=bt;
  fIsOK = (b%2);
  b /= 2;
  fIsCANCEL  = (b%2);
  b /= 2;
  fIsAPPLY  = (b%2);
  b /= 2;
  fIsREFRESH  = (b%2);
  b /= 2;
  fIsQUIT  = (b%2);
}
//______________________________________________________________________________
Bool_t PGInput::inpYN(const Char_t *question) 
{
  // Yes/No message box
  Int_t ret;
  EMsgBoxIcon icon = kMBIconQuestion;
  new TGMsgBox(gClient->GetRoot(),gClient->GetRoot(), 
	       "Question", question, icon, 3, &ret);
  if (ret==2) return 0;
  return 1;
}
//______________________________________________________________________________
Int_t PGInput::MessageBox(const Char_t *text) 
{
  // message box
  Int_t ret;
  EMsgBoxIcon icon = (EMsgBoxIcon) kMBIgnore;
  new TGMsgBox(gClient->GetRoot(),gClient->GetRoot(), 
	       "Question", text, icon, 0, &ret);
  return ret;
}
//______________________________________________________________________________
Int_t PGInput::SendInfo(const Char_t *info, Int_t line, bool autoupdate) 
{  
  // copy the new info and update the field
  // if autoupdate=1 update immediately the information
  if (strlen(info)>79) {
    printf("too long info!\n");
    return -1;
  }
  if (line<0 || line>= fNoInfo) {
    printf("wrong line number\n");
    return -2;
  }
  Int_t nwid = fInfoLine[line];
  //printf("PGInput::SendInfo() - nwid=%d  - info=%s\n",nwid,info); // zpe
  strcpy(fQuestion[nwid],info);
  if (fIsWin && autoupdate)
    fWin->UpdateField(nwid);
 
  return 0;
}
//______________________________________________________________________________
void PGInput::RefreshComboList(Int_t idx)
{
  // Regenerate the combo list (widget idx) according with new entry list

  if (!fIsWin) return;
  if (idx<0 || idx>=fNoItems) return;

  Int_t ffrom=idx;
  Int_t fto=idx+1;
  if (idx<0) {
    ffrom=0;
    fto=GetNoItems();
  }  
  Int_t nentries,j;
  for (Int_t i=ffrom; i<fto; i++) {
    switch (fType[i]) {

    case 'C': // Combo box
      fWin->fCombo[i]->RemoveEntries(0,MAXENTRIES);
      nentries=0;
      while (fEntryList[i][nentries] &&
	     fEntryList[i][nentries].CompareTo("EOE")) nentries++;	
      if (nentries>MAXENTRIES) nentries=MAXENTRIES;
      fComboNoEntries[i] = nentries;
      for (j=0; j < nentries; j++) 
	fWin->fCombo[i]->AddEntry(fEntryList[i][j], j);
      fWin->fCombo[i]->Select(fComboSelection[i]); // default selection
      break;
    }
  }
}
//______________________________________________________________________________
Int_t PGInput::GetNoComboEntries(Int_t idx)
{
  if (idx<0 || idx>=fNoItems) return -1;
  if (fType[idx] != 'C')
    return 0;
  return fComboNoEntries[idx];
}
//______________________________________________________________________________
void PGInput::ChangeButtonFace(Int_t idx, Char_t *text, Char_t *color,
			       Int_t w, Int_t h)
{
  if (idx<0 || idx>MAXPGLINES || !fWin) return;
  ULong_t tmp = 0;
  if (!fWin->fButton[idx]) return;

  if (color) {
    gClient->GetColorByName(color,tmp);
    fWin->fButton[idx]->ChangeBackground(tmp);
  }
  if (text) {
    fWin->fButton[idx]->SetText(text);
  }
  Int_t wi,he;
  if (w) wi=w; else wi=fWin->fButton[idx]->GetDefaultWidth();
  if (h) he=h; else he=GetPixelY();
  fWin->fF1[idx]->UnmapWindow(); 
  fWin->fF1[idx]->Resize(wi+10,he+20);
  fWin->fButton[idx]->Resize(wi,he);
  fWin->fF1[idx]->MapWindow();
}
//______________________________________________________________________________
//void PGInput::ChangeCommentColor(Int_t it, Char_t *color)
//{
//  if (it<0 || it>MAXPGLINES || !fWin) return;
//  ULong_t tmp = 0;
//  gClient->GetColorByName(color,tmp);
//  fWin->fLabel[it]->ChangeBackground(tmp);  
//  fWin->fF1[it]->UnmapWindow(); 
//  fWin->fF1[it]->MapWindow();
//}
//______________________________________________________________________________
TGTextButton *PGInput::GetButton(Int_t idx)
{
  if (idx<0 || idx>MAXPGLINES || !fWin) return NULL;
  return fWin->fButton[idx];
}
//______________________________________________________________________________
TGCheckButton *PGInput::GetCheckButton(Int_t idx)
{
  if (idx<0 || idx>MAXPGLINES || !fWin) return NULL;
  return fWin->fCheck[idx];
}
//______________________________________________________________________________
TGCompositeFrame *PGInput::GetFrame(Int_t idx)
{
  if (idx<0 || idx>MAXPGLINES || !fWin) return NULL;
  return fWin->fF1[idx];
}
//______________________________________________________________________________
void PGInput::SetFont(const Char_t *name)
{
  fontsel = gClient->GetFontByName(name);
}
//______________________________________________________________________________

Int_t PGInput::InputFilename(Char_t *fname, const Char_t *ext, const Char_t *mode) 
{
  // open file selector for open/save
  // return   0  if success
  //          1  if Cancel

  Char_t st[40];
  Char_t ext2[40];
  Char_t *filetypes[6];
  EFileDialogMode tipo=kFDOpen;
  strcpy(ext2,ext);
  Int_t le = strlen(ext2);
  if (!le) {
    strcpy(ext2,"*");
    le=1;
  }
  if (strstr(mode,"sa")) tipo=kFDSave;

  strcpy(st,ext2);
  //PtoUpper(st);
  strcat(st," files");
  Int_t ls = strlen(st);
  filetypes[0] = new Char_t[ls+1];
  strcpy(filetypes[0],st);

  strcpy(st,ext2);
  for (Int_t i=le+1; i>0; i--) st[i]=st[i-2];
  st[le+2] = 0;
  st[0] = '*';
  st[1] = '.';
  filetypes[1] = new Char_t[le+3];
  strcpy(filetypes[1],st);

  filetypes[2] = new Char_t[10];
  strcpy(filetypes[2],"all files");

  filetypes[3] = new Char_t[2];
  strcpy(filetypes[3],"*");

  filetypes[4] = 0;
  filetypes[5] = 0;

  TGFileInfo *fi = new TGFileInfo;
  fi->fFileTypes = (const char **)filetypes;
  new TGFileDialog(gClient->GetRoot(),gClient->GetRoot() , tipo, fi);

  Bool_t iscancel = 0;

  if (fi->fFilename) {
    strcpy(fname,fi->fFilename);
    if (!strstr(fname,".")) {
      strcat(fname,".");
      strcat(fname,ext);
    }
  }
  else
    iscancel = 1;

  for (Int_t i=0; i<4; i++) delete filetypes[i];
  delete fi;

  if (iscancel) return 1;

  return 0;
}
//______________________________________________________________________________

Int_t PGInput::InputFilenames(Char_t *fname, const Char_t *ext1, const Char_t *ext2, const Char_t *ext3, const Char_t *mode) 
{
  // open file selector for open/save
  // return   0  if success
  //          1  if Cancel
  // receives an array of extension strings, lasto one must be NULL
  
  Char_t st[40];
  Char_t exttmp[40];
  Char_t *filetypes[10];
  EFileDialogMode tipo=kFDOpen;
  if (strstr(mode,"sa")) tipo=kFDSave;

  int nlines=0;

  // prima ext
  strcpy(exttmp,ext1);
  Int_t le = strlen(exttmp);
  if (!le) {
    strcpy(exttmp,"*");
    le=1;
  }
  strcpy(st,exttmp);
  strcat(st," files");
  Int_t ls = strlen(st);
  filetypes[nlines] = new Char_t[ls+1];
  strcpy(filetypes[nlines],st);
  strcpy(st,exttmp);
  for (Int_t i=le+1; i>0; i--) st[i]=st[i-2];
  st[le+2] = 0;
  st[0] = '*';
  st[1] = '.';
  nlines++;
  filetypes[nlines] = new Char_t[le+3];
  strcpy(filetypes[nlines],st);
  nlines++;

  // seconda ext
  strcpy(exttmp,ext2);
  le = strlen(exttmp);
  if (le) {
    strcpy(st,exttmp);
    strcat(st," files");
    ls = strlen(st);
    filetypes[nlines] = new Char_t[ls+1];
    strcpy(filetypes[nlines],st);
    strcpy(st,exttmp);
    for (Int_t i=le+1; i>0; i--) st[i]=st[i-2];
    st[le+2] = 0;
    st[0] = '*';
    st[1] = '.';
    nlines++;
    filetypes[nlines] = new Char_t[le+3];
    strcpy(filetypes[nlines],st);
    nlines++;
  }

  // terza ext
  strcpy(exttmp,ext3);
  le = strlen(exttmp);
  if (le) {
    strcpy(st,exttmp);
    strcat(st," files");
    ls = strlen(st);
    filetypes[nlines] = new Char_t[ls+1];
    strcpy(filetypes[nlines],st);
    strcpy(st,exttmp);
    for (Int_t i=le+1; i>0; i--) st[i]=st[i-2];
    st[le+2] = 0;
    st[0] = '*';
    st[1] = '.';
    nlines++;
    filetypes[nlines] = new Char_t[le+3];
    strcpy(filetypes[nlines],st);
    nlines++;
  }
  
  filetypes[nlines] = new Char_t[10];
  strcpy(filetypes[nlines],"all files");
  nlines++;
  
  filetypes[nlines] = new Char_t[2];
  strcpy(filetypes[nlines],"*");
  nlines++;
  
  filetypes[nlines++] = 0;
  filetypes[nlines++] = 0;

  TGFileInfo *fi = new TGFileInfo;
  fi->fFileTypes = (const char **)filetypes;
  new TGFileDialog(gClient->GetRoot(),gClient->GetRoot() , tipo, fi);

  Bool_t iscancel = 0;

  if (fi->fFileTypeIdx==0) strcpy(exttmp,ext1);
  else if (fi->fFileTypeIdx==1) strcpy(exttmp,ext2);
  else if (fi->fFileTypeIdx==2) strcpy(exttmp,ext3);
  
  if (fi->fFilename) {
    strcpy(fname,fi->fFilename);
    if (!strstr(fname,".")) {
      strcat(fname,".");
      strcat(fname,exttmp);
    }
  }
  else
    iscancel = 1;

  for (Int_t i=0; i<(nlines-2); i++) delete filetypes[i];
  delete fi;

  if (iscancel) return 1;

  return 0;
}
//______________________________________________________________________________


void PGInput::Usersub1(Int_t choice, Long_t *par) { printf("\nvoid virtual Usersub1 invoked...\n\n"); int pippo=choice+(int)par[0];pippo++;}
void PGInput::Usersub2(Int_t choice, Long_t *par) { printf("\nvoid virtual Usersub2 invoked...\n\n");  int pippo=choice+(int)par[0];pippo++;}
void PGInput::Usersub3(Int_t choice, Long_t *par) { printf("\nvoid virtual Usersub3 invoked...\n\n");  int pippo=choice+(int)par[0];pippo++;}
void PGInput::Usersub4(Int_t choice, Long_t *par) { printf("\nvoid virtual Usersub4 invoked...\n\n");  int pippo=choice+(int)par[0];pippo++;}
void PGInput::Usersub5(Int_t choice, Long_t *par) { printf("\nvoid virtual Usersub5 invoked...\n\n");  int pippo=choice+(int)par[0];pippo++;}
void PGInput::Usersub6(Int_t choice, Long_t *par) { printf("\nvoid virtual Usersub6 invoked...\n\n");  int pippo=choice+(int)par[0];pippo++;}
void PGInput::Usersub7(Int_t choice, Long_t *par) { printf("\nvoid virtual Usersub7 invoked...\n\n");  int pippo=choice+(int)par[0];pippo++;}
void PGInput::Usersub8(Int_t choice, Long_t *par) { printf("\nvoid virtual Usersub8 invoked...\n\n");  int pippo=choice+(int)par[0];pippo++;}
void PGInput::Usersub9(Int_t choice, Long_t *par) { printf("\nvoid virtual Usersub9 invoked...\n\n");  int pippo=choice+(int)par[0];pippo++;}
//______________________________________________________________________________
void PGInput::SetUsersubPtr(PGInput *pg)
{
  fUsersubPtr=pg;
}


//---- Main program ------------------------------------------------------------
#ifdef STANDALONE
int main(int argc, char **argv)
{
   TApplication theApp("App", &argc, argv);

   if (gROOT->IsBatch()) {
      fprintf(stderr, "%s: cannot run in batch mode\n", argv[0]);
      return 1;
   }

   // ADD WINDOW BUILDING HERE

   theApp.Run();

   return 0;
}
#endif

////////////////////////////////////////////////////////////////////////////

class PCanvasContextMenu;  

class PMarker : public TObject {
public:
  Float_t fPos;  
  Int_t   fBin;
  TLine   fLine;
  Int_t   fColor;
  Int_t   fWidth;

  PMarker();
  virtual ~PMarker();

  void MDelete();
  void MCreate(Float_t pos, Int_t bin, TFrame *f, Bool_t log);
  void MDraw(TFrame *f, Bool_t log);
  void MMove(Float_t newpos, TFrame *f);
  void operator=(const PMarker &rhs);
};

class PCanvas : public TCanvas {
private:
  //    Added By PhoolChand on 15-11-99
  TPaveText *fPaveTextVolume;   // To display Volume !phool


  Bool_t   fButton1Down;
  Bool_t   fButton1ShiftDown;
  Bool_t   fButton2Down;
  Bool_t   fButton3Down;

  Int_t    fLastpx;
  Int_t    fLastpy;

  Bool_t   fSecondKey; 
  Int_t    fFirstKey;

  TFrame  *fLastFrame;

  TH1     *fH1;
  TH2F    *fH2;
  TH2     *fMatrix;
  Char_t   fHistName[60];
  Char_t   fMatName[60];
  TObject *fLastObj;
  Char_t   fObjectType[60];

  Double_t  fBck0;
  Double_t  fBck1;
  TF1      *fBckCh;
  TF1      *fBckChDraw;
  Float_t   fCal0; // X axis calibration coeff.
  Float_t   fCal1;

//----------------------------------------------------
  // markers
  Int_t   fNoMrkType;   // 'i','b','r','g'
  PMarker fMrk[10][20]; // 20 possible markers of 10 different types
  Int_t   fMaxMrk[10]; 
  Int_t   fNoMrk[10];
  Int_t   fMrkCol[10];
//----------------------------------------------------

  Float_t fPeak[20];
  Float_t fFWHM[20];
  Float_t fArea[20];
  Float_t fPeakErr[20];
  Float_t fFWHMErr[20];
  Float_t fAreaErr[20];

  TF1    *fFitFunc;
  TF1    *fFitFuncDraw;

  TLatex *fMV;
  
  Int_t   fMarker;
  Bool_t  fSecondMarker;

  Float_t fX;
  Float_t fY;
  Int_t   fbinX;
  Int_t   fbinY;

  Char_t  fNome[60];
  Bool_t  fLog;

  TGString fOptDraw[4];
  TGString fOptSmooth[3];
  Int_t    fOptSmoothSelection;
  Int_t    fOptSmoothRange;

  Char_t   fMatMainAxis; // X, Y
  Float_t  fTmpBck;

protected:
  PCanvasContextMenu *fPM;   //!  do not stream
  PGInput *pgopt; //!  do not stream
  Bool_t  fBckExp;
  Bool_t  fFitBck;
  Double_t fFixedFwhm;
  Bool_t  fFitFwhm;
  Int_t   fDrawAfterLoad;  // 0=don't draw   1=draw   2=draw same

  Int_t   CreateMarker(Float_t pos, Int_t bin, Int_t type);
  void    DeleteMarker(Int_t type);
  void    DrawMarker(Int_t type);
  Int_t   MrkType(Char_t type);  // 'i','b','r','g' 0=all
  void    UpdateMrks();
  void    SortMrks(Int_t type);
  void    SwapMrks(PMarker& m1, PMarker& m2);
  void    PrintMsg(Char_t *msg);
  Float_t EvalFWHM();
  void    CalcCalib();
  void    Init();

  Double_t PfitGauss(Double_t *x, Double_t *par);

 
public:
  PCanvas(const Char_t *nome="", const Char_t *titolo = "PCanvas", Int_t wtopx = 10, Int_t wtopy = 10, 
	  Int_t ww = 800, Int_t wh = 600);
  virtual ~PCanvas();

  Bool_t GetBckExp() {return fBckExp;}

  Bool_t GetFitBck() {return fFitBck;}

  Bool_t GetFitFwhm() {return fFitFwhm;}

  Double_t GetFixedFwhm() {return fFixedFwhm;}
  void SetFixedFwhm(Double_t fw=0) {fFixedFwhm=fw;}

  Bool_t  IsTH1();
  Bool_t  IsTH2();
  Bool_t  IsMatrix();
  Int_t   DefineMatrix(Char_t *name=NULL);
  void    DrawMatProje();

  virtual void ProcessContextMenu(Int_t selection);
  virtual void BuildContextMenu();

  virtual void BuildExtraContextMenu();
  virtual void ProcessExtraContextMenu(Int_t selection) { selection=0;}


  Float_t CalcInt(Int_t bck=0); // bck=1 = bck subtracion
  Int_t   CalcBck(Int_t plot=1);
  void    SetExpBck(Bool_t exp = kTRUE);
  void    SetFitBck(Bool_t fit = kTRUE) {fFitBck = fit;}
  void    SetFitFwhm(Bool_t fw = kTRUE) {fFitFwhm = fw;}
  Int_t   MakeSlice();

  Int_t   FitGauss();
  Int_t   GetGaussPar(Float_t& peak, Float_t& fwhm, Float_t& area, Int_t peakNo=0);
  
  virtual void HandleInput(EEventType event, Int_t x, Int_t y);
  void   HandleKeyboard(Int_t keycode, Int_t px, Int_t py);

  //    Added By PhoolChand on 15-11-99
  void   DisplayVolume(Char_t *vol);  

  void   CalVol(const char *name="CUTG");
  void   DeleteCutG(Int_t idx = -1); // -1 = All 2D gates
  TCutG *GetCutG(Int_t idx);
  void   PrintCutG(); 

  TH1   *GetH1() {return fH1;}
  TH2F  *GetH2() {return fH2;}
  
};


class PCanvasContextMenu : public TGFrame {  
public:
  PCanvasContextMenu(const TGWindow* p, UInt_t w = 10, UInt_t h = 10, UInt_t options = 0);
  virtual ~PCanvasContextMenu();

  TGPopupMenu *pm; 
  PCanvas *pcanvas;
  Int_t selection;

  virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t);
};

///////////////////////////////////////////////////////////////////////////////////
// utility functions
//__________________________________________________________________________
void Prompt()
{
  printf("%s",((TRint*)gApplication)->GetPrompt());
  fflush(stdout);
}
//__________________________________________________________________________
int inpSimple(const char *question, char &c, const char *choices=NULL)
{
  PGInput *pg = new PGInput;
  if (!pg) return -2;
  printf("please, answer in the box...\n");
  pg->AddComment("                  ");
  pg->AddEditBox(question, c);
  pg->Show((EPGInputButtonType)(kPG_OK|kPG_CANCEL), 1);
  delete pg;
  pg = NULL;
  if (choices)
    if (!strchr(choices,c)) return -1;
  return 0;
}
//______________________________________________________________________________
int inpSimple(const char *question, char *st, const char *choices=NULL)
{
  PGInput *pg = new PGInput;
  if (!pg) return -2;
  printf("please, answer in the box...\n");
  pg->AddComment("                  ");
  pg->AddEditBox(question, st, 16);
  pg->Show((EPGInputButtonType)(kPG_OK|kPG_CANCEL), 1);
  delete pg;
  pg = NULL;
  if (choices)
    if (!strstr(choices, st)) return -1;
  return 0;
}
//______________________________________________________________________________
int inpSimple(const char *question, int &i)
{
  PGInput *pg = new PGInput;
  if (!pg) return -2;
  printf("please, answer in the box...\n");
  pg->AddComment("                  ");
  pg->AddEditBox(question, i, 8);
  pg->Show((EPGInputButtonType)(kPG_OK|kPG_CANCEL), 1);
  delete pg;
  pg = NULL;
  return 0;
}
//______________________________________________________________________________
int inpSimple(const char *question, float &f)
{
  PGInput *pg = new PGInput;
  if (!pg) return -2;
  printf("please, answer in the box...\n");
  pg->AddComment("                  ");
  pg->AddEditBox(question, f, 8);
  pg->Show((EPGInputButtonType)(kPG_OK|kPG_CANCEL), 1);
  delete pg;
  pg = NULL;
  return 0;
}
//______________________________________________________________________________
int inpSimple(const char *question, double &d)
{
  PGInput *pg = new PGInput;
  if (!pg) return -2;
  printf("please, answer in the box...\n");
  pg->AddComment("                  ");
  pg->AddEditBox(question, d, 8);
  pg->Show((EPGInputButtonType)(kPG_OK|kPG_CANCEL), 1);
  delete pg;
  pg = NULL;
  return 0;
}
//______________________________________________________________________________

// -----------------------------------------------
Int_t saveHist(TH1 *h, Char_t *fname, Int_t attr=0)
{
  // attr=0 : don't overwrite

  if (!h) return -1;
  
  FILE *pf = NULL;
  if (attr==1) {
    pf = fopen(fname,"r");
    if (pf) {
      printf("file exists, cannot write...\n");
      return -1;
    }
  }
  

  pf = fopen(fname,"w");
  if (!pf) {
    printf("cannot open file!\n");
    return -2;
  }
  if (attr==1) {
    fprintf(pf,"# histo options:\n");
    fprintf(pf,"#   Name       %s\n",h->GetName());
    fprintf(pf,"#   Title      %s\n",h->GetTitle());
    fprintf(pf,"#   LineColor  %d\n",h->GetLineColor());
    fprintf(pf,"#   LineWidth  %d\n",h->GetLineWidth());
    fprintf(pf,"#   Xmin       %f\n",h->GetXaxis()->GetXmin());
    fprintf(pf,"#   Xmax       %f\n",h->GetXaxis()->GetXmax());
    fprintf(pf,"# histo data:\n");
  }

  for (Int_t i=0; i<h->GetNbinsX(); i++)
    fprintf(pf,"%f\n", h->GetBinContent(i+1));
  
  fclose(pf);
  return 0;
}
// -----------------------------------------------

// -----------------------------------------------
Int_t saveGraph(TGraph *g, Char_t *fname, Int_t attr=0)
{
  // save a TGraph in ascii format

  if (!g) return -1;
  
  FILE *pf = NULL;
  if (attr==1) {
    pf = fopen(fname,"r");
    if (pf) {
      printf("file exists, cannot write...\n ");
      return -1;
    }
  }

  pf = fopen(fname,"w");
  if (!pf) {
    printf("cannot open file!\n");
    return -2;
  }
  
  if (attr==1) {
    fprintf(pf,"# graph options:\n");
    fprintf(pf,"#   Name         %s\n",g->GetName());
    fprintf(pf,"#   Title        %s\n",g->GetTitle());
    fprintf(pf,"#   LineColor    %d\n",g->GetLineColor());
    fprintf(pf,"#   LineWidth    %d\n",g->GetLineWidth());
    fprintf(pf,"#   MarkerStyle  %d\n",g->GetMarkerStyle());
    fprintf(pf,"#   MarkerColor  %d\n",g->GetMarkerColor());
    fprintf(pf,"#   MarkerSize   %f\n",g->GetMarkerSize());
    fprintf(pf,"# graph data:\n");
  }

  for (Int_t i=0; i<g->GetN(); i++)
    fprintf(pf,"%f  %f\n", g->GetX()[i], g->GetY()[i]);

  fclose(pf);
  return 0;
}
// -----------------------------------------------
Int_t saveGraphErrors(TGraphErrors *g, const char *fname, Int_t attr=0)
{
  // save a TGraph in ascii format
  if (!g) return -1;
  
  FILE *pf = NULL;
  if (attr==1) {
    pf = fopen(fname,"r");
    if (pf) {
      printf("file exists, cannot write...\n");
      return -1;
    }
  }
  pf = fopen(fname,"w");
  if (!pf) {
    printf("cannot open file!\n");
    return -2;
  }

  
  if (attr==1) {
    fprintf(pf,"# graph options:\n");
    fprintf(pf,"#   Name         %s\n",g->GetName());
    fprintf(pf,"#   Title        %s\n",g->GetTitle());
    fprintf(pf,"#   LineColor    %d\n",g->GetLineColor());
    fprintf(pf,"#   LineWidth    %d\n",g->GetLineWidth());
    fprintf(pf,"#   MarkerStyle  %d\n",g->GetMarkerStyle());
    fprintf(pf,"#   MarkerColor  %d\n",g->GetMarkerColor());
    fprintf(pf,"#   MarkerSize   %f\n",g->GetMarkerSize());
    fprintf(pf,"# graph data:\n");
  }

  for (Int_t i=0; i<g->GetN(); i++)
    fprintf(pf,"%f  %f  %f  %f\n", g->GetX()[i], g->GetY()[i],
	    g->GetEX()[i], g->GetEY()[i]);

  fclose(pf);
  return 0;
}
//--------------------------------------------------------------------------
void PCheckName(char* s) {
  // check for a good root name
  for (Int_t i=0; i<(int)strlen(s); i++) {
    if (s[i]==95) continue; // '_'
    if (s[i]<48) s[i]='_';
    if (s[i]>57 && s[i]<65) s[i]='_';
    if (s[i]>90 && s[i]<97) s[i]='_';
    if (s[i]>122) s[i]='_';
  }
  if (s[0]<65) s[0]='_'; // start with a number
}
// -----------------------------------------------
TH1F *inpHist(Char_t *fname, Char_t *histname=NULL)
{
  // if histname="" use fname
  
  FILE *pf = NULL;
  pf = fopen(fname,"r");
  if (!pf) {
    printf("file not found!\n");
    return NULL;
  }

  Char_t hn[256*4]="";
  
  //if (!strcmp(histname,"")) {
  if (!histname) {
    char *pio = strrchr(fname,'/');    
    if (pio) strcpy(hn,++pio);
    else strcpy(hn,fname);    
    Int_t pospt = strcspn(hn,".");
    if (pospt>90) pospt=10;
    hn[pospt] = 0;
    hn[pospt+1] = 0;
  }  
  else 
    strcpy(hn,histname);

  PCheckName(hn);
  
  TH1F *h = NULL;
  Float_t y[16384];
  Float_t tmp[10];
  Char_t st[256];
  Char_t stmp[30];
  Char_t titolo[100];
  strcpy(titolo,"");
  Int_t n=0;

  Int_t lcol = 1;
  Int_t lwid = 1;
  Float_t xmin = 0;
  Float_t xmax = 0;

  while (fgets(st,255,pf)) {
    if (st[0] == '#') {
      if (strstr(st,"Name"))
	sscanf(st,"%s %s %s",stmp, stmp, hn);
      if (strstr(st,"Title")) {
	int n = sscanf(st,"%s %s %s",stmp, stmp,stmp);
	if (n>2) {
	  strcpy(titolo,strstr(st,stmp));
	  char *ret = index(titolo,'\n');
	  if (ret) {*ret=0; *(ret+1)=0;}
	  strcat(titolo," ");
	}
      }
      if (strstr(st,"LineColor"))
	  sscanf(st,"%s %s  %d",stmp, stmp, &lcol);
      if (strstr(st,"LineWidth"))
	  sscanf(st,"%s %s  %d",stmp, stmp, &lwid);
      //if (strstr(st,"NbinsX"))
      //  sscanf(st,"%s %s  %d",stmp, stmp, &nbins);
      if (strstr(st,"Xmin"))
	  sscanf(st,"%s %s  %f",stmp, stmp, &xmin);
      if (strstr(st,"Xmax"))
	  sscanf(st,"%s %s  %f",stmp, stmp, &xmax);
    }
    else {
      Int_t ndata=sscanf(st,"%f %f %f %f %f %f %f %f %f %f",
			 &tmp[0],&tmp[1],&tmp[2],&tmp[3],&tmp[4],
			 &tmp[5],&tmp[6],&tmp[7],&tmp[8],&tmp[9]);
   
      for (Int_t j=0; j<ndata; j++) y[n++]=tmp[j];
    }
  }
  fclose(pf);

  //printf("npts = %d\n",n);
  
  if (gROOT->FindObject(hn)) gROOT->FindObject(hn)->Delete();
  h = new TH1F(hn,titolo,n,0,n);
  h->SetBinContent(0,0);
  for (Int_t i=0; i<n; i++) {
    h->SetBinContent(i+1,y[i]);
  }
  h->SetLineColor(lcol);
  h->SetLineWidth(lwid);
  if (h->GetNbinsX() && (xmin<xmax)) 
    h->GetXaxis()->Set(h->GetNbinsX(),xmin,xmax);
  h->SetEntries(h->Integral());

  sprintf(st,"TH1F *%s = (TH1F*)gROOT->FindObject(\"%s\");",hn,hn);
  gROOT->ProcessLine(st);
  return h;
}
// -----------------------------------------------
TGraph *inpGraph( char *fname,  char *graphname=NULL)
{
  std::ifstream infile;
  infile.open(fname);
 
  if (infile.fail()) {
    printf("file not found!\n");
    return NULL;
  }

  Char_t titolo[256];
  strcpy(titolo,"");

  Char_t hn[256];

  if (!strlen(graphname)) {
    char *pio = strrchr(fname,'/');
    if (pio) strcpy(hn,++pio);
    else strcpy(hn,fname);
    Int_t pospt = strcspn(hn,".");
    if (pospt>90) pospt=10;
    hn[pospt] = 0;
    hn[pospt+1] = 0;
  }  
  else 
    strcpy(hn,graphname);

  PCheckName(hn);

  Char_t st[100];
  Char_t stmp[30];
  Int_t n=0;

  Int_t lcol = 2;
  Int_t lwid = 1;

  Int_t mcol = 4;
  Int_t msty = 20;
  Float_t msiz = 0.8;

  while (infile.getline(st,256)) {
    if (st[0] == '#') {
      if (strstr(st,"Name"))
	sscanf(st,"%s %s %s",stmp, stmp, hn);
      if (strstr(st,"Title")) {
	int n = sscanf(st,"%s %s %s",stmp, stmp,stmp);
	if (n>2) {
	  strcpy(titolo,strstr(st,stmp));
	  char *ret = index(titolo,'\n');
	  if (ret) {*ret=0; *(ret+1)=0;}
	  strcat(titolo," ");
	}
      }
      if (strstr(st,"LineColor"))
	  sscanf(st,"%s %s %d", stmp, stmp, &lcol);
      if (strstr(st,"LineWidth"))
	  sscanf(st,"%s %s %d", stmp, stmp, &lwid);
      if (strstr(st,"MarkerStyle"))
	  sscanf(st,"%s %s %d", stmp, stmp, &msty);
      if (strstr(st,"MarkerColor"))
	  sscanf(st,"%s %s %d", stmp, stmp, &mcol);
      if (strstr(st,"MarkerSize"))
	  sscanf(st,"%s %s %f", stmp, stmp, &msiz);
    }
    else {
    }
  }

  if (gROOT->FindObject(hn)) gROOT->FindObject(hn)->Delete();
  TGraph *g = new TGraph(fname);

  g->SetName(hn);
  g->SetTitle(titolo);
  g->SetLineColor(lcol);
  g->SetLineWidth(lwid);
  g->SetMarkerStyle(msty);
  g->SetMarkerColor(mcol);
  g->SetMarkerSize(msiz);

  return g;
}
// -----------------------------------------------
TGraphErrors *inpGraphErrors( char *fname,  char *graphname=NULL)
{  
  std::ifstream infile;
  infile.open(fname);
 
  if (infile.fail()) {
    printf("file not found!\n");
    return NULL;
  }
  
  Char_t titolo[100];
  strcpy(titolo,"");
  
  Char_t hn[256];
  
  if (!strlen(graphname)) {
    char *pio = strrchr(fname,'/');
    if (pio) strcpy(hn,++pio);
    else strcpy(hn,fname);
    Int_t pospt = strcspn(hn,".");
    if (pospt>90) pospt=10;
    hn[pospt] = 0;
    hn[pospt+1] = 0;
  }  
  else 
    strcpy(hn,graphname);
  
  PCheckName(hn);
  
  Char_t st[100];
  Char_t stmp[30];
  Int_t n=0;

  Int_t lcol = 2;
  Int_t lwid = 1;

  Int_t mcol = 4;
  Int_t msty = 20;
  Float_t msiz = 0.8;

  while (infile.getline(st,256)) {
    if (st[0] == '#') {
      if (strstr(st,"Name"))
	sscanf(st,"%s %s %s",stmp, stmp, hn);
      if (strstr(st,"Title")) {
	int n = sscanf(st,"%s %s %s",stmp, stmp,stmp);
	if (n>2) {
	  strcpy(titolo,strstr(st,stmp));
	  char *ret = index(titolo,'\n');
	  if (ret) {*ret=0; *(ret+1)=0;}
	  strcat(titolo," ");
	}
      }
      if (strstr(st,"LineColor"))
	  sscanf(st,"%s %s %d", stmp, stmp, &lcol);
      if (strstr(st,"LineWidth"))
	  sscanf(st,"%s %s %d", stmp, stmp, &lwid);
      if (strstr(st,"MarkerStyle"))
	  sscanf(st,"%s %s %d", stmp, stmp, &msty);
      if (strstr(st,"MarkerColor"))
	  sscanf(st,"%s %s %d", stmp, stmp, &mcol);
      if (strstr(st,"MarkerSize"))
	  sscanf(st,"%s %s %f", stmp, stmp, &msiz);
    }
    else {
      //sscanf(st,"%f %f %f %f",&x[n],&y[n],&ex[n],&ey[n]);
      //n++;
    }
  }


  if (gROOT->FindObject(hn)) gROOT->FindObject(hn)->Delete();
  TGraphErrors *g = new TGraphErrors(fname);

  g->SetName(hn);
  g->SetTitle(titolo);
  g->SetLineColor(lcol);
  g->SetLineWidth(lwid);
  g->SetMarkerStyle(msty);
  g->SetMarkerColor(mcol);
  g->SetMarkerSize(msiz);

  return g;
}
// -----------------------------------------------

// -----------------------------------------------
TH1 *smoothHist(TH1 *hist, const char *meth="average", Int_t nch=3, Char_t *hname=NULL)
{
  // smooth TH1F histogram with average or savitzky-golay method

  TH1 *h=NULL;
  if (nch>99) return NULL;
  if (!(nch%2)) nch++;
  
  Char_t tipo=' ';
  if (!strcmp(hist->ClassName(),"TH1S")) tipo='s';
  if (!strcmp(hist->ClassName(),"TH1F")) tipo='f';
  if (!strcmp(hist->ClassName(),"TH1D")) tipo='d';
  if (!strcmp(hist->ClassName(),"TH1C")) tipo='c';
  if (tipo==' ') return NULL;

  Int_t dim = hist->GetNbinsX();

  Double_t *p = new Double_t[dim+202];
  if (!p) {
    printf("error allocating memory!");
    return NULL;
  }
  Double_t x = 0;
  TRandom rand;

  // copy bin contents shifted of 100: first bin is p[101]
  for (Int_t i=0; i<=100; i++) p[i]=hist->GetBinContent(1);
  for (Int_t i=1; i<=dim; i++) 
    p[i+100]=hist->GetBinContent(i);
  for (Int_t i=dim+1; i<dim+100; i++) p[i]=hist->GetBinContent(dim);

  if (!hname) h = hist;
  else {
    if (gROOT->FindObject(hname)) gROOT->FindObject(hname)->Delete();
    switch(tipo) {
    case 's': h = new TH1S(*((TH1S*)hist)); break;
    case 'f': h = new TH1F(*((TH1F*)hist)); break;
    case 'd': h = new TH1D(*((TH1D*)hist)); break;
    case 'c': h = new TH1C(*((TH1C*)hist)); break;
    }
    h->SetName(hname);
    h->Reset();
  }
  if (!h) {
    printf("error allocating histogram!");
    delete [] p;
    return NULL;
  }

  // smoothing with average
  if (strstr(meth,"ave")) {
    if (!(nch%2)) nch++; // No of nch must be odd
    for (Int_t i=101; i<(dim+101); i++) {
      x = 0;
      for (Int_t j=0; j<nch; j++) x += p[i-int(nch/2)+j];
      x /= float(nch);
      h->SetBinContent(i-100,x);
    }
  }
  // smoothing with Savitzky-Golay (9pts, 4degree)
  else if (strstr(meth,"sav")) {
    Float_t savgol[9] = 
    {0.035,-0.128,0.070,0.315,0.416,0.315,0.070,-0.128,0.035};
    for (Int_t i=101; i<(dim+101); i++) {
      x = 0;
      for (Int_t j=0; j<9; j++) x += p[i-4+j]*savgol[j];
      h->SetBinContent(i-100,x);
    }
  }

  delete [] p;

  return h;
}
// -----------------------------------------------


//______________________________________________________________________________
Double_t (*tmpfunc)(Double_t *, Double_t *); // global temp pointer for func
TF1* tmpbck=0;
//______________________________________________________________________________
Double_t PfitGauss2(Double_t *x, Double_t *par)
{
  // par[0] = fBckExp*100 + n. of peaks
  // par[1] = fBck0
  // par[2] = fBck1
  // FWHM(1)=par[5];  FWHM(2)=par[5]+par[8]; ...

  Int_t tmp = int(par[0]);
  Int_t bcktype = tmp/100;
  Int_t npeaks = tmp%100;

  Double_t bg=0;
  if (!bcktype) bg = par[1]+x[0]*par[2];
  else bg = TMath::Exp(par[1]+x[0]*par[2]);

  Double_t fitval = 0;

  for (Int_t i=1; i<=npeaks; i++) {
    Double_t arg = 0;
    Double_t gconst = 0;

    Double_t sigma = par[5];
    if (i>1) sigma += (par[i*3+2]+0.5);
    sigma /= 2.35;

    if (sigma) {
      arg = (x[0] - par[i*3+1])/sigma;
      gconst = par[i*3]/(sigma*2.507);
    }

    fitval += gconst*TMath::Exp(-0.5*arg*arg);
  }
  fitval += bg;
  return fitval;
}
//______________________________________________________________________________


PCanvas::PCanvas(const Char_t *nome, const Char_t *titolo, Int_t wtopx, Int_t wtopy, Int_t ww, Int_t wh) : 
TCanvas(nome, titolo, wtopx, wtopy, ww, wh) {
  TString nm=this->GetName();
  strcpy(fNome,nm.Data());
  Init();
}

void PCanvas::Init() 
{
  // initialize custom variable
  fButton1Down = kFALSE;
  fButton1ShiftDown = kFALSE;
  fButton2Down = kFALSE;
  fButton3Down = kFALSE;
  fSecondKey = kFALSE;
  fLastpx = 0;
  fLastpy = 0;
  fFirstKey = 0;

  fLastFrame = NULL;
  fLastObj = NULL;

  fH1 = NULL;
  fH2 = NULL;
  fMatrix = NULL;
  strcpy(fObjectType,"");
  strcpy(fHistName,"");
  strcpy(fMatName,"");

  fBck0 = 0.0;
  fBck1 = 1.0;
  fBckCh = NULL;
  fBckChDraw = NULL;
  fFitFunc = NULL;
  fFitFuncDraw = NULL;
  fCal0 = 0.0;
  fCal1 = 1.0;
  fTmpBck = 0;
  //--------------------------------
  for (Int_t jj=0; jj<10; jj++) {
    fMaxMrk[jj]=0;
    fNoMrk[jj]=0;
    fMrkCol[jj]=0;
  }

  fNoMrkType = 4;
  fMaxMrk[0] = 2;  fMrkCol[0] = 8;   // markers i
  fMaxMrk[1] = 4;  fMrkCol[1] = 4;   // markers b
  fMaxMrk[2] = 2;  fMrkCol[2] = 2;   // markers r
  fMaxMrk[3] = 10;  fMrkCol[3] = 6;  // markers g

  for (Int_t i=0; i<fNoMrkType; i++) {
    for (Int_t j=0; j<fMaxMrk[i]; j++) {
      fMrk[i][j].fColor = fMrkCol[i];     
    }
  }
  //------------------------------
  for (Int_t i=0; i<20; i++) {
    fPeak[i]=0;
    fFWHM[i]=0;
    fArea[i]=0;
    fPeakErr[i]=0;
    fFWHMErr[i]=0;
    fAreaErr[i]=0;
  }

  fMV = NULL;
  
  fMarker = 0;
  fSecondMarker = kFALSE;

  fX = 0;
  fY = 0;
  fbinX = 0;
  fbinY = 0;

   fPaveTextVolume=NULL;
  fLog = kFALSE;
  fBckExp = 0;
  fFitBck=0;
  fFitFwhm = 0;
  fFixedFwhm = 0;
  fDrawAfterLoad = 1;
  fPM = NULL;
  fMatMainAxis = 'X';

  pgopt = new PGInput;
  fOptDraw[0]="Do Not Draw";
  fOptDraw[1]="Draw()";
  fOptDraw[2]="Draw(\"SAME\")";
  fOptDraw[3]="EOE";
  
  fOptSmooth[0]="average";
  fOptSmooth[1]="savitzky-golay";
  fOptSmooth[2]="EOE";

  fOptSmoothSelection=0;
  fOptSmoothRange=5;

  pgopt->AddComment("Multi Gaussian Fit");
  pgopt->AddEditBox("Exponential Background ",fBckExp);
  pgopt->AddEditBox("Fit Background ",fFitBck);
  pgopt->AddEditBox("Fit FWHM ",fFitFwhm);
  pgopt->AddEditBox("Fix FWHM at ",fFixedFwhm,8);
  pgopt->AddComment(" ");
  pgopt->AddComment("1D Hist/Graph");
  pgopt->AddComboBox("After Loading: ",fOptDraw,fDrawAfterLoad);
  pgopt->AddComment(" ");
  pgopt->AddComboBox("Smooth method: ",fOptSmooth,fOptSmoothSelection);
  pgopt->AddEditBox("over ch. : ",fOptSmoothRange,3);
  pgopt->AddComment(" ");
  pgopt->SetButtonColor("grey80");
  pgopt->SetCommentColor("red");
  pgopt->SetTitle("PCanvas Options");

  
}
//______________________________________________________________________________
PCanvas::~PCanvas() {
  if (fPM) delete fPM;
  delete pgopt;
}





//______________________________________________________________________________
Bool_t PCanvas::IsTH1()
{
  if (!fH1) return 0;
  if (!gROOT->FindObject(fHistName)) {
    fH1=0;
    return 0;  
  }
  if (!strstr(gROOT->FindObject(fHistName)->ClassName(),"TH1")) {
    fH1=0;
    return 0;
  }
  fH1 = (TH1*) gROOT->FindObject(fHistName);
  fH2 = NULL;
  return 1;
}
//______________________________________________________________________________
Bool_t PCanvas::IsTH2()
{
  if (!fH2) return 0;
  if (!gROOT->FindObject(fHistName)) {
    fH2=0;
    return 0;  
  }
  if (!strstr(gROOT->FindObject(fHistName)->ClassName(),"TH2")) {
    fH2=0;
    return 0;
  }
  fH2 = (TH2F*) gROOT->FindObject(fHistName);
  fH1 = NULL;
  return 1;
}
//______________________________________________________________________________
Bool_t PCanvas::IsMatrix()
{
  if (!fMatrix) return 0;
  if (!gROOT->FindObject(fMatName)) {
    fMatrix=0;
    return 0;  
  }
  if (!strstr(gROOT->FindObject(fMatName)->ClassName(),"TH2")) {
    fMatrix=0;
    return 0;
  }
  fMatrix = (TH2*) gROOT->FindObject(fMatName);
  return 1;
}
//______________________________________________________________________________
Int_t PCanvas::DefineMatrix(Char_t *name)
{
  Char_t nome[60];

  if (IsMatrix())
    printf("... have already a matrix with name %s\n",fMatName);

  if (name) 
    strcpy(nome,name);
  else
    strcpy(nome,fMatName);

  inpSimple("Matrix (2D histo) name: (q=quit)",nome);
  if ( !strlen(nome) ) return 1;
  if ( !strcmp(nome,"q") ) return 1;

  strcpy(fMatName,nome);

  fMatrix = (TH2*) gROOT->FindObject(fMatName);
  if (!IsMatrix()) {
    strcpy(fMatName,"");
    fMatrix = NULL;
    printf("couldn't find object %s or it's not a 2D histogram\n",nome);
    return -1;
  }
  inpSimple("Main axis? ",fMatMainAxis,"XY");

  DrawMatProje();
  return 0;
}
//______________________________________________________________________________
void PCanvas::DrawMatProje()
{
  if (!IsMatrix()) {
    printf("No matrix defined. Use 'dq' to define one\n");
    return;
  }
  
  TH1D *pro = (TH1D*) gROOT->FindObject("PCanvasMatProje");
  if (pro) gROOT->FindObject("PCanvasMatProje")->Delete();
  
  if (fMatMainAxis == 'X')
    pro = fMatrix->ProjectionX("PCanvasMatProje");
  else
    pro = fMatrix->ProjectionY("PCanvasMatProje");
  if (!pro) return;
  pro->Draw();
  Modified();
  Update();
  fH1 = pro;
  fH2 = NULL;
  strcpy(fHistName,"PCanvasMatProje");
}
//______________________________________________________________________________
void PCanvas::HandleInput(EEventType event, Int_t px, Int_t py) {
  
  TCanvas::HandleInput(event,px,py);       

  if (GetPadSave()->GetFrame() != fLastFrame){
    fLastFrame = GetPadSave()->GetFrame();    
  }

  // PICK UP HISTOGRAMS
  if (fSelected && (strstr(fSelected->ClassName(),"TH1") ||
		    strstr(fSelected->ClassName(),"TH2")) ) {
    // c'e' un histo
    Bool_t cegia=1;

    if (fSelected != fLastObj) // diverso da prima
      cegia=0;    
    else { // uguale a prima -> anche LastObj != 0
      if ((strstr(fSelected->ClassName(),"TH1") && fH2) ||
	  (strstr(fSelected->ClassName(),"TH2") && fH1) ) 
	cegia=0;
    }

    if (!cegia) {
      if (strstr(fSelected->ClassName(),"TH1")) { // l'oggetto e' un TH1
	fH1 = (TH1*) fSelected;
	fLastObj = fSelected;
	fH2 = NULL;
	printf("found new histogram 1D with name %s \n",fH1->GetName());
	Prompt();
	strcpy(fHistName,fH1->GetName());
	CalcCalib();
      }
      else if (strstr(fSelected->ClassName(),"TH2")) { // l'oggetto e' un TH2
	fH2 = (TH2F*) fSelected;
	fLastObj = fSelected;
	fH1 = NULL;
	printf("found new TH2F with name %s \n",fH2->GetName());
	Prompt();
	strcpy(fHistName,fH2->GetName());
      }
    }
    
  }

  //printf("event = %d\n",(int)event);

  
  switch (event) {
    
  case kMouseEnter: // ?
    break;
    
  case kMouseMotion: // 51
    fLastpx = px;
    fLastpy = py;
    break;
    
  case kButton1Down:   // 1
    // by Phool !27/12/99
    TObjLink *tmpo;
    if ( !Pick(px,py,tmpo) ) return;
    gPad = Pick(px,py,tmpo);
    fButton1Down=kTRUE;
    break;
    
  case kButton1Motion: // 21
    break;
    
  case kButton1Up:    // 11
    fButton1Down=kFALSE;
    if (fButton1ShiftDown) {
      fButton1ShiftDown=kFALSE;
      fButton2Down=kFALSE;
    }
    break;
    
  case kButton2Down:  //2
    fButton2Down=kTRUE;
    // ---------------------
    if (!fPM) BuildContextMenu();
    fPM->pm->PlaceMenu(px+GetWindowTopX(),py+20+GetWindowTopY(),0,1);
    // ---------------------
    break;

  case kButton1Shift:  // 7 = shift + tasto sinistro
    fButton2Down=kTRUE;
    fButton1ShiftDown=kTRUE;    
    // ---------------------
    if (!fPM) BuildContextMenu();
    fPM->pm->PlaceMenu(px+GetWindowTopX(),py+20+GetWindowTopY(),0,1);
    // ---------------------
    break;
    
  case kButton2Up:  // 12
    fButton2Down=kFALSE;
    break;
    
  case kButton3Down: // 3
    fButton3Down=kTRUE;
    break;
    
  case kButton3Up:  // missing here
    fButton3Down=kFALSE;
    break;

  case 24: // keyboard pressed code px
    HandleKeyboard(px,fLastpx,fLastpy);
    break;

  case 61: // double click button 1 in the sequence: down-up-61-up 
    break;    

  case 62: // double click button 2 
    break;

  case 63: // double click button 3 
    break;

  case kMouseLeave: // mouse leave 
    // force popdown of tooltips
    {
    }
    break;
    
  default:
    break;    
  }

}
//______________________________________________________________________________
void PCanvas::HandleKeyboard(Int_t keycode, Int_t px, Int_t py) {
  // px,py sono le coordinate nella canvas
  // fX,fY                    nel frame
  // fbinX, fbinY             i bins
  // i limiti del frame sono fFrame->[GetX1() - GetX2()] e [GetY1() - GetY2()]
  
  if (GetPadSave()->GetFrame()) {
    fX =  GetPadSave()->AbsPixeltoX(px); 
    fY =  GetPadSave()->AbsPixeltoY(py);
    if (fX<GetPadSave()->GetFrame()->GetX1() || fX>GetPadSave()->GetFrame()->GetX2()) return;
    if (fY<GetPadSave()->GetFrame()->GetY1() || fY>GetPadSave()->GetFrame()->GetY2()) return;
  }    
  else {
    fX = 0; fY = 0;
  }

  if (IsTH1()) {
    fbinX = fH1->GetXaxis()->FindBin( GetPadSave()->AbsPixeltoX(px)) - 1;
    fbinY = 0;
  }
  else if (IsTH2()) {
    fbinX = fH2->GetXaxis()->FindBin( GetPadSave()->AbsPixeltoX(px)) - 1;
    fbinY = fH2->GetYaxis()->FindBin( GetPadSave()->AbsPixeltoY(py)) - 1;
  }      

  Char_t code;
  Char_t msg[80];

  // start switching SECOND keycodes
  if (fSecondKey) {
    fSecondKey = kFALSE;
    switch (fFirstKey) {

    case 102:  // f pressed first
      switch (keycode) {
	// use UnZoom instead of ff - fx - fy
      }
      break;

    case 'z': // delete markers
      if (IsTH1()) {
	code = Char_t(keycode);
	switch (code) {
	case 'z':
	  DeleteMarker(-1);
	  Update();
	  break;
	case 'v':
	  if (fMV) delete fMV;
	  fMV=0;
	  Update();
	  break;
	default:
	  DeleteMarker(MrkType(code));
	  Update();
	  break;
	}
      }
      break;

    case 'm': // show markers
      if (IsTH1()) {
	switch (keycode) {
	case 'z':
	  UpdateMrks();
	  break;
	default:
	  DrawMarker(MrkType((Char_t) keycode));
	  break;
	}
      }
      break;

    case 'c':
      switch (keycode) {
      case 'i': //integral
	if (IsTH1()) CalcInt();
	break;
      case 'j': //integral
	if (IsTH1()) CalcInt(1);
	break;
      case 'b': //bckground
	if (IsTH1()) CalcBck();
	break;
      case 'g': //gauss
	if (IsTH1()) FitGauss();
	break;
      case 'w': // coinc
	if (IsMatrix()) MakeSlice();
	break;
      }
      break;

    case 'd':
      switch (keycode) {
      case 'q':
	DefineMatrix();
	break;
      }
      break;
      
    case 107: // k pressed first
      printf("pressed %d at (%d,%d)\n",keycode,px,py);
      break;
    }
  }

  // start switching FIRST keycodes
  else {
    switch(keycode){

    case 32:  // spacebar 
      break;

    case 'c':  // calculate area
      fSecondKey = kTRUE;
      fFirstKey = 'c';      
      break;

    case 'd': // define matrix
      fSecondKey = kTRUE;
      fFirstKey = 'd';      
      break;

    case 'q': // show projection matix
      DrawMatProje();
      break;

    case 102: // 'f' 
      fSecondKey = kTRUE;
      fFirstKey = 102;      
      break;

    case 107:  // 'k' = show keycode
      printf("press any key to see the keycode... \n");
      fSecondKey = kTRUE;
      fFirstKey = 107;
      break;

    case 'l': 
      fLog = !fLog;
      SetLogy(fLog);
      Modified();
      Update();
      UpdateMrks();
      break;

    case 'p': // coordinates
      printf("\n(x,y) = (%d,%d) in pixel\n",px,py);
      printf("(x,y) = (%f,%f) in frame\n",fX,fY);
      break;

    case 'm': // show markers
      fSecondKey = kTRUE;
      fFirstKey = 'm';      
      break;
      
    case 'v':
      if (IsTH1()) {
	printf("binx = %d, content = %.0f\n",fbinX, fH1->GetBinContent(fbinX+1));
      }
      else if (IsTH2()) {
	printf("binx = %d,  biny= % d,  content = %.0f\n",fbinX, fbinY, 
	       fH2->GetBinContent(fbinX+1, fbinY+1));
      }
      break;

      // markerZ

    case 'g':
      CreateMarker(fX,fbinX,keycode);
      SortMrks(MrkType(keycode));
      break;
    case 'i':
      CreateMarker(fX,fbinX,keycode);
      SortMrks(MrkType(keycode));
      break;
    case 'r':
      CreateMarker(fX,fbinX,keycode);
      SortMrks(MrkType(keycode));
      break;
    case 'b':
      CreateMarker(fX,fbinX,keycode);
      SortMrks(MrkType(keycode));
      break;


    case 'z':
      fSecondKey = kTRUE;
      fFirstKey = 'z';      
      break; 
      
    default:
      break;
    }
  }
}
//______________________________________________________________________________
void PCanvas::DeleteCutG(Int_t idx)
{
  TCutG *cut = GetCutG(idx);
  if (cut) {
    cut->Delete();
    printf("%d CutG removed\n",idx);
    return;
  }
  printf("couldn't find CutG n. %d\n",idx);
  return;
}
//______________________________________________________________________________
TCutG *PCanvas::GetCutG(Int_t idx)
{
  TSeqCollection *cutgCol = gROOT->GetListOfSpecials();
  TIter next(cutgCol);
  TObject *obj;
  Int_t ncutg=0;

  while ((obj=next())) {    
    if (!strcmp(obj->ClassName(),"TCutG") ) {
      if (ncutg==idx) return (TCutG*) obj;
      ncutg++;
    }
  }
  return NULL;
}
//______________________________________________________________________________
void PCanvas::CalVol(const char *name)
{
  if (!IsTH2()) return; // no TH2 found

  TH2F *mth2f = fH2;

  Float_t maxX=0, maxY=0;
  Float_t minX=1000000, minY=1000000;

  TCutG *mycutgA[32];
  TCutG *cut = NULL;
  Int_t tot_cut=0;
  Int_t idx=0;

  while ( (cut=GetCutG(idx++)) ) {
    if (!strcmp(cut->GetName(), name)) {
      mycutgA[tot_cut] = cut;    
      Double_t *pointX = mycutgA[tot_cut]->GetX();
      Double_t *pointY = mycutgA[tot_cut]->GetY();

      Int_t npoints = mycutgA[tot_cut]->GetN();
      
      for ( Int_t i = 0; i < npoints; i++ )
	{
	  if ( minX > pointX[i] ) minX = pointX[i];
	  if ( maxX < pointX[i] ) maxX = pointX[i];
	  if ( minY > pointY[i] ) minY = pointY[i];
	  if ( maxY < pointY[i] ) maxY = pointY[i];
	}
      tot_cut++;
    }
  }
  if (!tot_cut) {
    printf("Couldn't find any TCutG  with name %s \n", name);
    return;
  }

  TAxis *xAxis = mth2f->GetXaxis();
  TAxis *yAxis = mth2f->GetYaxis();
  Int_t nx = xAxis->GetNbins();
  Int_t ny = yAxis->GetNbins();
  
  Int_t firstxbin = xAxis->FindBin(minX);
  Int_t lastxbin = xAxis->FindBin(maxX);
  Int_t firstybin = yAxis->FindBin(minY);
  Int_t lastybin = yAxis->FindBin(maxY);
  
  if (firstxbin < 0) firstxbin = 0;
  if (lastxbin > nx) lastxbin = nx;
  if (firstybin < 0) firstybin = 0;
  if (lastybin > ny) lastybin = ny;
  
  Float_t xmin = xAxis->GetXmin();
  Float_t xmax = xAxis->GetXmax();
  Float_t ymin = yAxis->GetXmin();
  Float_t ymax = yAxis->GetXmax();
  
  Float_t ConCoeffX  =  (xmax - xmin)/float(nx); 
  Float_t ConCoeffY  =  (ymax - ymin)/float(ny);
  
  Long_t entries = 0;
  Float_t py  =  ymin+ (firstybin -1)*ConCoeffY; 
  Float_t pxstart  =  xmin+ (firstxbin -1)*ConCoeffX; 
  
  for (Int_t biny=firstybin; biny<=lastybin+1; biny++) {
    py = py + ConCoeffY;
    Float_t px  = pxstart;
    for (Int_t binx=firstxbin; binx<=lastxbin+1;binx++) {
      Int_t inside=0;
      px = px + ConCoeffX;
      for ( Int_t i = 0; i < tot_cut; i++ ) {
	if ( strcmp(mycutgA[i]->GetName(), name) ) continue; 
	if ( mycutgA[i]->IsInside(px, py) ) {
	  inside = 1;
	  break;
	}
      }
      if ( inside ) {
	Int_t cont  = mth2f->GetCellContent(binx,biny);
	entries += cont;
      }
    }
  }

  printf( "Total Volume %ld \n", entries );
  return;
}

//______________________________________________________________________________
void PCanvas::PrintCutG() 
{
  TCutG *cut = NULL;
  Int_t idx=0;
  printf("cut#\tname\tn. pt.\n");
  printf("---------------------------\n");
  while ( (cut=GetCutG(idx)) ) 
    printf("%d\t%s\t%d\n",idx++,cut->GetName(),cut->GetN());  
  printf("---------------------------\n");
}
//______________________________________________________________________________
void PCanvas::DisplayVolume(Char_t *vol)
{
  if (  !gPad->GetPrimitive("PaveTextVolume") ) {
     fPaveTextVolume = new TPaveText(1500, 4300, 2500, 4550);
     fPaveTextVolume->SetName("PaveTextVolume"); 
  }

  fPaveTextVolume->Clear();
  fPaveTextVolume->AddText(vol);
  fPaveTextVolume->Draw();

}
//______________________________________________________________________________
Int_t PCanvas::CreateMarker(Float_t pos, Int_t bin, Int_t type)
{
  type = MrkType(type);
  if (type==-1 || !IsTH1()) return -1;
  if (fNoMrk[type] >= fMaxMrk[type]) return -1;
  fMrk[type][fNoMrk[type]].MCreate(pos,bin,GetPadSave()->GetFrame(),GetLogy());
  fNoMrk[type]++;
  Update();
  return 0;
}
//______________________________________________________________________________
void  PCanvas::DeleteMarker(Int_t type)
{
  if (type==-1) {
    for (Int_t i=0; i<fNoMrkType; i++) DeleteMarker(i);
    return;
  }
  if (type<0 || type>=fNoMrkType) return;
  for (Int_t i=0; i<fNoMrk[type]; i++) fMrk[type][i].MDelete();
  fNoMrk[type]=0;

  if (fBckCh) { 
    delete fBckCh;
    fBckCh = NULL;
  }
  if (fBckChDraw) { 
    delete fBckChDraw;
    fBckChDraw = NULL;
  }

  if (fFitFunc) { 
    delete fFitFunc;
    fFitFunc = NULL;
  }
  if (fFitFuncDraw) { 
    delete fFitFuncDraw;
    fFitFuncDraw = NULL;
  }

  Modified();
  Update();  
  return;
}
//______________________________________________________________________________
void  PCanvas::SortMrks(Int_t type)
{
  if (type==-1) {
    for (Int_t i=0; i<fNoMrkType; i++) SortMrks(i);
    return;
  }

  Int_t max;
  Int_t imax;

  for (Int_t j=fNoMrk[type]-1; j>0; j--) {
    max = -9999;
    imax = 0;
    for (Int_t i=0; i<=j; i++) {
      if (fMrk[type][i].fBin > max) {
	max = fMrk[type][i].fBin;
	imax = i;
      }
    }
   
    SwapMrks(fMrk[type][j], fMrk[type][imax]);
  }
  return;
}
//______________________________________________________________________________
Int_t PCanvas::MrkType(Char_t t)
{
  // i=0  b=1  r=2  g=3   all=0
  switch (t) {
  case 'i':
    return 0;
  case 'b':
    return 1;
  case 'r':
    return 2;
  case 'g':
    return 3;
  case 0:
    return -1;
  }
  return -99;
}
//______________________________________________________________________________
void PCanvas::UpdateMrks() 
{
  for (Int_t i=0; i<fNoMrkType; i++) DrawMarker(i);
}
//______________________________________________________________________________
void PCanvas::DrawMarker(Int_t type) 
{
  if (type<0 || type>fNoMrkType) return;
  for (Int_t i=0; i<fNoMrk[type]; i++) fMrk[type][i].MDraw(GetPadSave()->GetFrame(), GetLogy());
  TCanvas::Update();
}
//______________________________________________________________________________
template <class T>
inline void zwap(T& a, T& b) {
  T tmp = a;
  a = b;
  b = tmp;
}
//______________________________________________________________________________
void PCanvas::SwapMrks(PMarker& m1, PMarker& m2) 
{
  zwap(m1.fPos, m2.fPos);
  zwap(m1.fBin, m2.fBin);
  zwap(m1.fLine, m2.fLine);
  zwap(m1.fColor, m2.fColor);
  zwap(m1.fWidth, m2.fWidth);
}
//______________________________________________________________________________
void PCanvas::PrintMsg(Char_t *msg)
{
  if (fMV) delete fMV;
  fMV = new TLatex(fX, fY, msg);
  fMV->SetTextSize(0.03);
  fMV->Draw();
  Update();	
}
//______________________________________________________________________________
Float_t PCanvas::CalcInt(Int_t bck)
{
  // return 0 if error

  fTmpBck = 0;

  if (!IsTH1()) return 0;

  if (bck<11 && fNoMrk[0]<2) return 0; 

  Int_t bfrom = fMrk[0][0].fBin+1;
  Int_t bto = fMrk[0][1].fBin+1;

  if (bck > 10) { // use 'r' markers
    bfrom = fMrk[2][0].fBin+1;
    bto = fMrk[2][1].fBin+1;
  }

  // calcola l'integrale
  Float_t integ = fH1->Integral(bfrom, bto);
  // calcola il centroide
  Float_t intx=0;
  for (Int_t j=bfrom; j<=bto; j++)
    intx += fH1->GetBinContent(j)*fH1->GetBinCenter(j);
  if (integ) intx /= integ;
  else intx = 0;

  if (bck==12) return integ;

  Float_t bg=0;    
  if (bck) {
    if (bck==11) CalcBck(0);
    else CalcBck();
    if (!fBckCh) return 0;
    intx=0;
    for (Int_t j=bfrom; j<=bto; j++) {      
      bg = bg + (Float_t) fBckCh->Eval((j+0.5));
      intx += (fH1->GetBinContent(j)-(Float_t) fBckCh->Eval((j+0.5)))*fH1->GetBinCenter(j);
    }
    integ -= bg;
    if (integ) intx /= integ;
    else intx = 0;    
  }

  if (bck==11) return integ;
  fTmpBck = bg;
  Float_t integerr=0, bgerr=0;
  if (bck) {
    integerr = TMath::Sqrt(integ+2*bg);
    bgerr = TMath::Sqrt(bg);
  }
  else
    integerr=TMath::Sqrt(integ);
  
  printf("integral = %.0f +- %.0f\n",integ,integerr);
  if (bck) printf("background = %.0f +- %.0f\n",bg,bgerr);
  printf("centroid = %.2f\n",intx);

  return integ;
}
//______________________________________________________________________________
Int_t PCanvas::MakeSlice()
{
  Float_t area = 0;
  if (fNoMrk[1]<2) 
    area = CalcInt(0);
  else 
    area = CalcInt(1); 

  if (!area) return -1; // error while calculating integral

  Int_t bfrom = fMrk[0][0].fBin+1; // marker 'i'
  Int_t bto = fMrk[0][1].fBin+1;
  
  Int_t b1from = fMrk[1][0].fBin + 1; // marker 'b'
  Int_t b1to   = fMrk[1][1].fBin + 1;
  Int_t b2from = fMrk[1][2].fBin + 1;
  Int_t b2to   = fMrk[1][3].fBin + 1;
  
  Modified();
  Update();
  gSystem->Sleep(200);

  // build gate slice
  TH1D *gate = (TH1D*) gROOT->FindObject("PCanvasMatGate");
  if (gate) gROOT->FindObject("PCanvasMatGate")->Delete();
  if (fMatMainAxis == 'X')   
    gate = fMatrix->ProjectionY("PCanvasMatGate",bfrom,bto);
  else
    gate = fMatrix->ProjectionX("PCanvasMatGate",bfrom,bto);
  if (!gate) return -4;
  TH1D *gatesub = (TH1D*) gROOT->FindObject("PCanvasMatGateSub");
  if (gatesub) gROOT->FindObject("PCanvasMatGateSub")->Delete();
  gatesub = new TH1D(*gate);
  gatesub->SetName("PCanvasMatGateSub");
  gatesub->SetLineColor(2);
  
  // bck slices
  TH1D *gateb1 = (TH1D*) gROOT->FindObject("PCanvasMatGateBck1");
  if (gateb1) gROOT->FindObject("PCanvasMatGateBck1")->Delete();
  gateb1 = 0;
  TH1D *gateb2 = (TH1D*) gROOT->FindObject("PCanvasMatGateBck2");
  if (gateb2) gROOT->FindObject("PCanvasMatGateBck2")->Delete();
  gateb2 = 0;
  TH1D *totbck = (TH1D*) gROOT->FindObject("PCanvasMatGateBck");
  if (totbck) gROOT->FindObject("PCanvasMatGateBck")->Delete();
  totbck = 0;
  
  // build first bck
  if (fNoMrk[1]>0) {
    if (fMatMainAxis == 'X')   
      gateb1 = fMatrix->ProjectionY("PCanvasMatGateBck1",b1from,b1to);
    else
      gateb1 = fMatrix->ProjectionX("PCanvasMatGateBck1",b1from,b1to);
    if (!gateb1) return -2;
    totbck = new TH1D(*gateb1);
    totbck->SetName("PCanvasMatGateBck");
    totbck->SetLineColor(9);
  }

  // build second bck
  if (fNoMrk[1]>2) {
    if (fMatMainAxis == 'X')   
      gateb2 = fMatrix->ProjectionY("PCanvasMatGateBck2",b2from,b2to);
    else
      gateb2 = fMatrix->ProjectionX("PCanvasMatGateBck2",b2from,b2to);    
    if (!gateb2) return -2;
    if (!totbck) return -3;
    totbck->Add(gateb2);
  }
 
  // normalize and subtract bck
  if (totbck) {
    totbck->Scale(fTmpBck/totbck->Integral());
    gatesub->Add(totbck,-1.0);
  }

  if (gateb1) delete gateb1;
  if (gateb2) delete gateb2;
    
  gatesub->Draw("hist");
  
  Modified();
  Update();
  fH1 = gate;
  fH2 = NULL;
  strcpy(fHistName,"PCanvasMatGate");  
  
  return 0;
}
//______________________________________________________________________________
Int_t PCanvas::FitGauss()
{
  // return 0 if success

  if (!IsTH1()) return -1;

  // check for kind of histogram
  Char_t tipo=' ';
  if (!strcmp(fH1->ClassName(),"TH1S")) tipo='s';
  if (!strcmp(fH1->ClassName(),"TH1F")) tipo='f';
  if (!strcmp(fH1->ClassName(),"TH1D")) tipo='d';
  if (!strcmp(fH1->ClassName(),"TH1C")) tipo='c';

  if (fNoMrk[2] < 2) {
    printf("error: not all 'r' markers defined\n");
    return -1;  // r
  }
  if (fNoMrk[3] < 1) {
    printf("error: not 'g' markers defined\n");
    return -1;  // g
  }

  Bool_t cebck=1;
  if (fNoMrk[1] < 4) { // b
    printf("\n   ***   not all 'b' markers defined   ***\n");
    printf("   ***         using no backgroud      ***\n\n");
    cebck = 0;
  }  

  Float_t r1 =  fMrk[2][0].fBin;
  Float_t r2 =  fMrk[2][1].fBin+1.0;

  if ( (fMrk[3][0].fBin <= r1) ||
       (fMrk[3][0].fBin >= r2) ) {
    printf("error: peak out of region\n");
    return -1;
  }
  

  // set approx init values

  // calculate total integral with bck subtraction
  Float_t peakarea = 0;
  if (cebck) peakarea = CalcInt(11); // init value is 'cj'
  else  peakarea = CalcInt(12);

  if (!peakarea) {
     printf("error evaluating inital area\n");
     return -1;
  }
  // now fBckCh is the TF1 of the bck

  // create temporary histogram with no calibration
  TH1 *htemp = NULL;
  if (tipo=='f') htemp = new TH1F(*((TH1F*)fH1));
  if (tipo=='d') htemp = new TH1D(*((TH1D*)fH1));
  if (tipo=='s') htemp = new TH1S(*((TH1S*)fH1));
  if (tipo=='c') htemp = new TH1C(*((TH1C*)fH1));

  if (!htemp) {
    printf("error creating temporary histogram\n");
    return -1;
  }
  htemp->SetName("PCanvas_htemp");
  Int_t realnbins = fH1->GetNbinsX();
  htemp->GetXaxis()->Set(realnbins,0,realnbins);
  htemp->GetXaxis()->SetRange(0,realnbins);
  // recalculate calibration coefficients
  CalcCalib();

  // create fit function
  if (gROOT->FindObject("PCanvasFitFunc")) 
    gROOT->FindObject("PCanvasFitFunc")->Delete();  // delete previuos func if exists
  Int_t npar = 3+fNoMrk[3]*3;
  tmpfunc =  PfitGauss2;
  fFitFunc = new TF1("PCanvasFitFunc", tmpfunc,r1,r2,npar);
  // create fit function to be drawn
  if (gROOT->FindObject("PCanvasFitFuncDraw")) 
    gROOT->FindObject("PCanvasFitFuncDraw")->Delete();  // delete previuos func if exists
  fFitFuncDraw = new TF1("PCanvasFitFuncDraw", 
			 tmpfunc, (fCal0+r1*fCal1),
			 (fCal0+fCal1*r2),npar);

  // calculate fwhm of first peak
  Float_t pwidth = EvalFWHM();
  // if pwidth fails, using region/NoPeaks
  if (!pwidth) pwidth=(r2-r1)/(fNoMrk[3]*2);
  if (fFixedFwhm) {
    if (fCal1) pwidth = fFixedFwhm/fCal1;
  }
  fFitFunc->SetParameter(5,pwidth); // set first fwhm

  Double_t par0 = fBckExp*100 + fNoMrk[3]; 
  fFitFunc->SetParameter(0,par0);
  fFitFunc->SetParLimits(0,par0,par0);  

  // init bck
  if (!cebck) { // no bck defined
    fBck0 = -1;
    fBck1 = -1;
    if (fBckCh) delete fBckCh;
    if (fBckChDraw) delete fBckChDraw;
    fBckCh = new TF1("fBckCh","0.0*x",0,htemp->GetXaxis()->GetXmax());
  }
  fFitFunc->SetParameter(1,fBck0);
  fFitFunc->SetParameter(2,fBck1);
  if (!fFitBck || !cebck) { // fix bck parameters
    fFitFunc->SetParLimits(1,fBck0,fBck0);
    fFitFunc->SetParLimits(2,fBck1,fBck1);
  } 

  // init peaks
  Float_t toth=0; // pepo: forse c'e' dell'abs...
  Float_t hparz[20];
  for (Int_t hh=0; hh<fNoMrk[3]; hh++) {
    hparz[hh] = htemp->GetBinContent(fMrk[3][hh].fBin+1);
    hparz[hh] -= fBckCh->Eval(fMrk[3][hh].fBin+0.5);
    toth += hparz[hh];
  }
  for (Int_t hh=0; hh<fNoMrk[3]; hh++) {
    //printf("A(%d)=%f\n",hh,peakarea*hparz[hh]/toth);
    fFitFunc->SetParameter((hh*3+3),peakarea*hparz[hh]/toth);
    fFitFunc->SetParameter((hh*3+4), (fMrk[3][hh].fBin + 0.5));
    if (hh) fFitFunc->SetParameter((hh*3+5),-0.5);
  }
  
  if (fFixedFwhm) {
    fFitFunc->SetParLimits(5,pwidth,pwidth); 
  }

  if (!fFitFwhm) {
    for (Int_t hh=1; hh<fNoMrk[3]; hh++) fFitFunc->SetParLimits(hh*3+5,-0.5,-0.5); 
  }

  htemp->Fit("PCanvasFitFunc","RIQN");

  fBck0 = fFitFunc->GetParameter(1);
  fBck1 = fFitFunc->GetParameter(2);

  // draw fit func
  Float_t paro;
  fFitFuncDraw->SetLineWidth(2);
  fFitFuncDraw->SetLineColor(13);
  fFitFuncDraw->SetBit(kCannotPick);
  fFitFuncDraw->SetParameter(0,fFitFunc->GetParameter(0));
  paro = (fBck0 - fCal0*fBck1/fCal1);
  fFitFuncDraw->SetParameter(1,paro);
  paro = fBck1/fCal1;
  fFitFuncDraw->SetParameter(2,paro);
  
  // get fitted parameters
  for (Int_t hh=0; hh<fNoMrk[3]; hh++) {
    fArea[hh] = fFitFunc->GetParameter(hh*3+3);
    fAreaErr[hh] = fFitFunc->GetParError(hh*3+3);
    fFitFuncDraw->SetParameter(hh*3+3,fArea[hh]*fCal1);

    fPeak[hh] = fCal0 + fFitFunc->GetParameter(hh*3+4)*fCal1;
    fPeakErr[hh] = fFitFunc->GetParError(hh*3+4)*fCal1;
    fFitFuncDraw->SetParameter(hh*3+4,fPeak[hh]);

    fFWHM[hh] = fFitFunc->GetParameter(hh*3+5)*fCal1;    
    fFWHMErr[hh] = fFitFunc->GetParError(hh*3+5)*fCal1;    
    fFitFuncDraw->SetParameter(hh*3+5,fFWHM[hh]);
    if (hh) {
      fFWHM[hh] = ( fFitFunc->GetParameter(5) + 0.5 +
		   fFitFunc->GetParameter(hh*3+5))*fCal1;
      fFWHMErr[hh] += fFWHMErr[0];
      fFitFuncDraw->SetParameter(hh*3+5,
				 (fFWHM[hh] - 0.5 - fFWHM[0]));
    }

    printf("--- GAUSSIAN FIT: peak n.%d -----------\n",hh+1);
    printf("     peak = %.4f +- %.4f\n",fPeak[hh],fPeakErr[hh]);
    printf("     fwhm = %.4f +- %.4f      resol.(perc.) = %.4f\n",
	   fFWHM[hh],fFWHMErr[hh],(100.*fFWHM[hh]/fPeak[hh]));
    printf("     area = %.1f +- %.1f\n",fArea[hh],fAreaErr[hh]);
    fMrk[3][hh].MMove(fPeak[hh], GetPadSave()->GetFrame());
  }
  printf("---------------------------------------\n");
  Double_t redchi2=fFitFunc->GetChisquare();
  if (fFitBck) redchi2 /= (r2-r1-1);
  else redchi2 /= (r2-r1-3);
  printf("     reduced chisquare: %.3f\n",redchi2);
  printf("---------------------------------------\n");

  fFitFuncDraw->Draw("same");  

  if (cebck) {
    fBckChDraw->SetParameters( (fBck0 - fBck1*fCal0/fCal1),
			       (fBck1/fCal1) );
    fBckChDraw->Draw("same");
  }
  Update();
  Prompt();
  
  delete htemp;
  return 0;
}
//______________________________________________________________________________
Int_t PCanvas::GetGaussPar(Float_t& peak, Float_t& fwhm, Float_t& area, Int_t peakNo)
{
  if (peakNo<0 || peakNo>=fMaxMrk[3]) return -1;
  peak = fPeak[peakNo];
  fwhm = fFWHM[peakNo];
  area = fArea[peakNo];
  return 0;
}
//______________________________________________________________________________
Int_t PCanvas::CalcBck(Int_t plot)
{
  // return 0 if success
  // calculate bck using one pair (horiz.) or two pairs (linear) of 'b' markers 
  
  if (!IsTH1()) return -1;
  if (fNoMrk[1]!=2 && fNoMrk[1]!=4) return -1;
  
  CalcCalib(); 
  if (!fCal1) return -1; // strange calibration

  Double_t b1 = 0;
  Double_t b2 = 0;
  Double_t xb1 = 0;
  Double_t xb2 = 0;

  // first bck point
  Int_t ifrom = fMrk[1][0].fBin + 1;
  Int_t ito   = fMrk[1][1].fBin + 1;
  for (Int_t i=ifrom; i<=ito; i++) b1 += fH1->GetBinContent(i);
  b1 /= (ito-ifrom+1);
  xb1 = (ito+ifrom+1)/2 - 0.5;

  // second bck point
  if (fNoMrk[1] == 4) { // two pairs
    ifrom = fMrk[1][2].fBin + 1;
    ito   = fMrk[1][3].fBin + 1;    
    for (Int_t i=ifrom; i<=ito; i++) b2 += fH1->GetBinContent(i);
    b2 /= (ito-ifrom+1);
    xb2 = (ito+ifrom+1)/2 - 0.5;
  }
  else { // one pairs
    b2 = b1;
    if (fNoMrk[0]<2) { // no I markers defined
      xb1 = 0.5;
      xb2 = fH1->GetNbinsX() - 0.5;
    }
    else {
      if (fMrk[0][1].fBin >= fMrk[1][1].fBin)
	xb2 = fMrk[0][1].fBin + 1.5;
      else
	xb2 = fMrk[0][0].fBin - 1.5;
    }
  }
  if (fBckExp) {
    b1 = TMath::Log(b1);
    b2 = TMath::Log(b2);
  }
  if (fBckCh) {
    delete fBckCh;
    fBckCh=NULL;
  }
  if (fBckChDraw) {
    delete fBckChDraw;
    fBckChDraw=NULL;
  }

  fBck1 = (b2-b1)/(xb2-xb1);
  fBck0 = b1 - xb1*fBck1;
  
  if (fBckExp) {
    fBckCh = new TF1("fBckCh","exp([0]+[1]*x)",xb1,xb2);    
    fBckChDraw = new TF1("fBckChDraw","exp([0]+[1]*x)",
			 (fCal0+fCal1*xb1),(fCal0+fCal1*xb2));
  }
  else {
    fBckCh = new TF1("fBckCh","[0]+[1]*x",xb1,xb2);
    fBckChDraw = new TF1("fBckChDraw","[0]+[1]*x",
			 (fCal0+fCal1*xb1),(fCal0+fCal1*xb2));
  }
  fBckCh->SetParameters(fBck0, fBck1);
  
  fBckChDraw->SetParameters( (fBck0 - fBck1*fCal0/fCal1),
			     (fBck1/fCal1) );
  fBckChDraw->SetLineWidth(1);
  fBckChDraw->SetLineColor(6);
  fBckChDraw->SetBit(kCannotPick);

  if (plot) {
    fBckChDraw->Draw("same");
    Update();
  }

  return 0;
}
//______________________________________________________________________________
Float_t PCanvas::EvalFWHM() 
{
  //evaluate width of first peak in the fit region
  Int_t ffrom =  fMrk[2][0].fBin+1;
  Int_t fto   =  fMrk[2][1].fBin+1;

  Float_t f1=0;
  Float_t f2=0;

  for (Int_t i=ffrom+1; i<fto; i++) {
    if (!f1) {
      Float_t d1 = fH1->GetBinContent(i) - fH1->GetBinContent(i-1);
      Float_t d2 = fH1->GetBinContent(i+1) - fH1->GetBinContent(i);
      if (d1>=0 && d2<d1) f1=i+0.5;
    }
    if (f1 && !f2) {
      Float_t d1 = fH1->GetBinContent(i) - fH1->GetBinContent(i-1);
      Float_t d2 = fH1->GetBinContent(i+1) - fH1->GetBinContent(i);
      if (d1<=0 && d2>d1) f2=i+0.5;
    }
  }

  return (f2-f1);
}
//______________________________________________________________________________
Double_t PCanvas::PfitGauss(Double_t *x, Double_t *par)
{
  Double_t arg = 0;
  Double_t sigma = par[2]/2.35;
  Double_t garea = par[0]/(sigma*2.507);
  if (par[2]) arg = (x[0] - par[1])/sigma;
  
  Double_t fitval = garea*TMath::Exp(-0.5*arg*arg) - fBckCh->Eval(x[0]);
  
  return fitval;
}
//______________________________________________________________________________
void PCanvas::CalcCalib()
{
  if (!IsTH1()) return;
  Int_t realnbins = fH1->GetNbinsX();
  fCal1 = (fH1->GetXaxis()->GetXmax() - fH1->GetXaxis()->GetXmin())/realnbins;
  fCal0 = fH1->GetXaxis()->GetXmin();
}
//______________________________________________________________________________
void PCanvas::SetExpBck(Bool_t exp)
{
  // *MENU*
  fBckExp = exp;
}
//______________________________________________________________________________
void PCanvas::BuildContextMenu()
{
  // default PCanvas ContextMenu for button 2
  if (fPM) return;

  fPM = new PCanvasContextMenu(gClient->GetRoot());
  fPM->pcanvas = this;
  fPM->pm->AddLabel("PCanvas Menu");
  fPM->pm->AddSeparator();
  fPM->pm->AddLabel("1D Hist/Graph:");
  fPM->pm->AddEntry("Save to text file",1);
  fPM->pm->AddEntry("Load TH1F from text file",2);
  fPM->pm->AddEntry("Load TGraph from text file",3);
  fPM->pm->AddEntry("Load TGraphErrors from text file",6);
  fPM->pm->AddEntry("Smooth Hist",4);
  fPM->pm->AddSeparator();
  fPM->pm->AddEntry("Options",5);
  BuildExtraContextMenu();
}
//______________________________________________________________________________
void PCanvas::BuildExtraContextMenu()
{
  // default PCanvas ContextMenu for button 2
  if (!fPM) return;
  fPM->pm->AddSeparator();
  fPM->pm->AddLabel("CutG:");
  fPM->pm->AddEntry("Draw CutG's",21);      
  fPM->pm->AddEntry("Calculate Volume",22);
}
//______________________________________________________________________________
void PCanvas::ProcessContextMenu(Int_t selection)
{
  //PInput p;
  Char_t filename[300];
  Char_t opt[30];
  strcpy(filename,"");
  TH1F   *ht = NULL;
  TGraph *gt = NULL;
  TGraphErrors *ge = NULL;
  TCutG *cut = NULL;
  Int_t idx  = 0;
  Int_t sel  = 0;

  switch (selection) {
  case 1: // save object
    if (strstr(fSelected->ClassName(),"TH1")) {
      sel = pgopt->InputFilename(filename,"txt","save");
      if (sel) return;
      if (saveHist((TH1*)fSelected, filename)) return;
      printf("histo saved to file!\n");
    }
    if (!strcmp(fSelected->ClassName(),"TGraph")) {
      sel = pgopt->InputFilename(filename,"txt","save");
      if (sel) return;
      if (saveGraph((TGraph*)fSelected, filename)) return;
      printf("graph saved to file!\n");
    }
    if (!strcmp(fSelected->ClassName(),"TGraphErrors")) {
      sel = pgopt->InputFilename(filename,"txt","save");
      if (sel) return;
      if (saveGraphErrors((TGraphErrors*)fSelected, filename)) return;
      printf("graph saved to file!\n");
    }
    Prompt();
    break;

  case 2:
    sel = pgopt->InputFilenames(filename,"txt","dat");
    if (sel) return;
    ht = inpHist(filename);
    if (!ht) return;
    if (!fDrawAfterLoad) return;
    if (fDrawAfterLoad==1) strcpy(opt,"");
    if (fDrawAfterLoad==2) strcpy(opt,"same");
    ht->Draw(opt);
    Modified();
    Update();
    break;

  case 3:
    sel = pgopt->InputFilenames(filename,"txt","dat");
    if (sel) return;
    gt = inpGraph(filename);

    if (!gt) return;
    if (!fDrawAfterLoad) return;
    if (fDrawAfterLoad==1) strcpy(opt,"APL");
    if (fDrawAfterLoad==2) strcpy(opt,"PL");
    gt->Draw(opt);
    Modified();
    Update();
    break;

  case 4:
    if (strstr(fSelected->ClassName(),"TH1")) {
      smoothHist( (TH1*)fSelected ,fOptSmooth[fOptSmoothSelection],fOptSmoothRange);
      Modified();
      Update();
    }
    break;
    
  case 5:
    pgopt->Show((EPGInputButtonType)(kPG_OK|kPG_CANCEL|kPG_APPLY),0);
    break;

  case 6:
    sel = pgopt->InputFilename(filename,"txt","dat");
    if (sel) return;
    ge = inpGraphErrors(filename);
    if (!ge) return;
    if (!fDrawAfterLoad) return;
    if (fDrawAfterLoad==1) strcpy(opt,"APL");
    if (fDrawAfterLoad==2) strcpy(opt,"PL");
    ge->Draw(opt);
    Modified();
    Update();
    break;

  case 21:
    while ( (cut=GetCutG(idx++)) ) cut->Draw();
    Modified();
    Update();
    break;

  case 22:
    if (strstr(fSelected->ClassName(),"TCutG")) {
      cut = (TCutG*) fSelected;      
      strcpy(filename,cut->GetName());
      cut->SetName("PCanvas_tempCutG");
      CalVol("PCanvas_tempCutG");
      cut->SetName(filename);
    }
    break;

  default:
    break;
  }
  ProcessExtraContextMenu(selection);
}
//______________________________________________________________________________
PMarker::PMarker() 
{
  fPos = 0;
  fBin = 0;
  //fLine = NULL;
  fLine.SetX1(0);
  fLine.SetX2(0);
  fLine.SetY1(0);
  fLine.SetY2(0);
  fColor = 1;
  fWidth = 1;
}
//______________________________________________________________________________
PMarker::~PMarker() {
  MDelete();
}
//______________________________________________________________________________
void PMarker::MCreate(Float_t pos, Int_t bin, TFrame *f, Bool_t log)
{
  fLine.SetX1(pos);
  fLine.SetX2(pos);
  fLine.SetBit(kCannotPick);
  fLine.SetLineColor(fColor);
  fLine.SetLineWidth(fWidth);

  fBin = bin;
  fPos = pos;
  MDraw(f,log);
}
//______________________________________________________________________________
void PMarker::MDraw(TFrame *f, Bool_t log) {
  double y1=f->GetY1();
  double y2=f->GetY2();
  //if (GetLogy()) {
  if (log) {
    y1 = TMath::Power(10,y1);
    y2 = TMath::Power(10,y2);
  }
  fLine.SetY1(y1);
  fLine.SetY2(y2);
  fLine.Draw();
}
//______________________________________________________________________________
void PMarker::MMove(Float_t newpos, TFrame *f) {
  if (!f) printf("boh!\n");
  fLine.SetX1(newpos);
  fLine.SetX2(newpos);
  fLine.Draw();
}
//______________________________________________________________________________
void PMarker::MDelete()
{
  fLine.SetX1(0);
  fLine.SetX2(0);
  fLine.SetY1(0);
  fLine.SetY2(0);
  fPos = 0;
  fBin = 0;
}
//______________________________________________________________________________
void PMarker::operator=(const PMarker &rhs)
{
  fPos = rhs.fPos;
  fBin = rhs.fBin;
  fLine = rhs.fLine;
  fColor = rhs.fColor;
  fWidth = rhs.fWidth;
}
//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
PCanvasContextMenu::PCanvasContextMenu(const TGWindow* p, UInt_t w, UInt_t h, UInt_t options) :
  TGFrame(p, w, h, options)
{ 
  pm = new TGPopupMenu(p,w,h,options);
  pm->Associate(this);
  pcanvas = NULL;
  selection = 0;
}
//______________________________________________________________________________
PCanvasContextMenu::~PCanvasContextMenu()
{
  delete pm;
}
//______________________________________________________________________________
Bool_t PCanvasContextMenu::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2)
{
  // Handle messages send to the TestMainFrame object. E.g. all menu button
  // messages.
  TGFrame::ProcessMessage(msg,parm1,parm2);
  
  switch (GET_MSG(msg)) {
    
  case kC_COMMAND:
    switch (GET_SUBMSG(msg)) {
      
    case kCM_BUTTON:
      printf("Button was pressed, id = %ld\n", parm1);
      break;
      
    case kCM_MENUSELECT:
      break;
      
    case kCM_MENU:
      selection = parm1;
      pcanvas->ProcessContextMenu(selection);
      break;      
      
    default:
      break;
    }

  default:
    break;
  }
  if (msg == 257) {
  }
  return kTRUE;
}
//______________________________________________________________________________
//______________________________________________________________________________
