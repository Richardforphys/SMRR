//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sun Mar 28 12:41:08 2010 by ROOT version 5.26/00
// from TTree Gas_Tree/Gas_Tree
// found on file: neutr9M_run1.root
//////////////////////////////////////////////////////////

#ifndef sel_baseclass_h
#define sel_baseclass_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>

class sel_baseclass : public TSelector {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain

   // Declaration of leaf types
   Int_t           ntracks;
   Int_t           id[160];   //[ntracks]
   Int_t           type[160];   //[ntracks]
   Int_t           mum[160];   //[ntracks]
   Float_t         edep[160];   //[ntracks]
   Float_t         zp[160];   //[ntracks]
   Float_t         t[160];   //[ntracks]
   Int_t           evid;

   // List of branches
   TBranch        *b_ntracks;   //!
   TBranch        *b_id;   //!
   TBranch        *b_type;   //!
   TBranch        *b_mum;   //!
   TBranch        *b_edep;   //!
   TBranch        *b_zp;   //!
   TBranch        *b_t;   //!
   TBranch        *b_evid;   //!

   sel_baseclass(TTree * /*tree*/ =0) { }
   virtual ~sel_baseclass() { }
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

   ClassDef(sel_baseclass,0);
};

#endif

#ifdef sel_baseclass_cxx
void sel_baseclass::Init(TTree *tree)
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
   fChain->SetBranchAddress("zp", zp, &b_zp);
   fChain->SetBranchAddress("t", t, &b_t);
   fChain->SetBranchAddress("evid", &evid, &b_evid);
}

Bool_t sel_baseclass::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef sel_baseclass_cxx
