//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Mar 19 10:07:10 2010 by ROOT version 5.24/00
// from TChain Gas_Tree/gas_Tree
//////////////////////////////////////////////////////////

#ifndef n_al_h
#define n_al_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TH1F.h>
#include <TCanvas.h>

class n_al : public TSelector {
  public :
    
  
    //TTree          *fChain;  //!pointer to the analyzed TTree or TChain
  TChain          *fChain;  //!pointer to the analyzed TTree or TChain
  //TChain         *fC;
  
 // Declaration of leaf types
 Int_t           ntracks;
   Int_t           id[100000];   //[ntracks] 133
   Int_t           type[100000];   //[ntracks]
   Int_t           mum[100000];   //[ntracks]
   Float_t         edep[100000];   //[ntracks]

   // List of branches
   TBranch        *b_ntracks;   //!
   TBranch        *b_id;   //!
   TBranch        *b_type;   //!
   TBranch        *b_mum;   //!
   TBranch        *b_edep;   //!

   n_al(TTree * /*tree*/ =0) { }
   virtual ~n_al() { }
   virtual Int_t   Version() const { return 2; }
   //virtual void    Begin(TChain *tree);
   //virtual void    SlaveBegin(TChain *tree);
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   //virtual void    Init(TTree *tree);
   virtual void    Init(TChain *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

   ClassDef(n_al,0);

   //Declaration of histograms for different particles energy deposition
   
   TH1F* hele;
   TH1F* hprot;
   TH1F* hgamma;
   TH1F* hneutr;
   TH1F* harg;
   //TH1F* hcarb;
   //TH1F* hoxy;
   //TH1F* hcopper;
   
   TCanvas* cele;
   TCanvas* cprot;
   TCanvas* cgamma;
   TCanvas* cneutr;
   TCanvas* carg;
   TCanvas* ccarb;
   TCanvas* coxy;
   TCanvas* ccopper;
   
   

};

#endif

#ifdef n_al_cxx
//void n_al::Init(TTree *tree)
void n_al::Init(TChain *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("ntracks", &ntracks, &b_ntracks);
   fChain->SetBranchAddress("id", id, &b_id);
   fChain->SetBranchAddress("type", type, &b_type);
   fChain->SetBranchAddress("mum", mum, &b_mum);
   fChain->SetBranchAddress("edep", edep, &b_edep);
}

Bool_t n_al::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef n_al_cxx
