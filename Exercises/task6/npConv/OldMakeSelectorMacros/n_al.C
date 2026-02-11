#define n_al_cxx
// The class definition in n_al.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.

// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// Root > T->Process("n_al.C")
// Root > T->Process("n_al.C","some options")
// Root > T->Process("n_al.C+")
//

#include "n_al.h"
#include <TH2.h>
#include <TStyle.h>


void n_al::Begin(TTree * /*tree*/)
//void n_al::Begin(TChain * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

   hele = new TH1F("ElectronEnDep","ElectronEnDep",2000,0,2000);
   hprot = new TH1F("ProtonEnDep","ProtonEnDep",2000,0,2000);
   hgamma = new TH1F("GammaEnDep","GammaEnDep",2000,0,2000);
   hneutr = new TH1F("NeutronEnDep","NeutronEnDep",2000,0,2000);
   harg = new TH1F("ArgonEnDep","ArgonEnDep",2000,0,2000);
   //hcarb = new TH1F("CarbonEnDep","CarbonEnDep",2000,0,2000);
   //hoxy = new TH1F("OxygenEnDep","OxygenEnDep",2000,0,2000);
   //hcopper = new TH1F("CopperEnDep","CopperEnDep",2000,0,2000);


   cele = new TCanvas("CanvEl","CanvEl");
   cprot = new TCanvas("CanvPr","CanvPr");
   cgamma = new TCanvas("CanvGam","CanvGam");
   cneutr = new TCanvas("CanvNeut","CanvNeutr");
   carg = new TCanvas("CanvArg","CanvArg");
   ccarb = new TCanvas("CanvCarb","CanvCarb");
   coxy = new TCanvas("CanvOxy","CanvOxy");
   ccopper = new TCanvas("CanvCop","CanvCop");
   
   Info("Begin","Histos and Canvas declared");

}

void n_al::SlaveBegin(TTree * /*tree*/)
//void n_al::SlaveBegin(TChain * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

Bool_t n_al::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either n_al::GetEntry() or TBranch::GetEntry()
   // to read either all or the required parts of the data. When processing
   // keyed objects with PROOF, the object is already loaded and is available
   // via the fObject pointer.
   //
   // This function should contain the "body" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.

  fChain->GetTree()->GetEntry(entry);

  Double_t ele_tedep = 0;
  Double_t prot_tedep = 0 ;
  Double_t gamma_tedep = 0;
  Double_t neutr_tedep = 0;
  Double_t arg_tedep = 0;
  Double_t carb_tedep = 0;
  Double_t oxy_tedep = 0;
  Double_t copper_tedep = 0;



  for(Int_t i=0; i<ntracks; i++)
    {
      //Identify Electrons
      if(type[i] == 11) 
	ele_tedep += edep[i];

      //Identify Protons
      if(type[i] == 2212) prot_tedep += edep[i];

      //Identify Neutrons
      if(type[i] == 2121) neutr_tedep += edep[i];

      //Identify Gamma
      if(type[i] == 22) gamma_tedep += edep[i];

      //Identify Argon Ion
      if(type[i] == 1000180400 || type[i] == 1000180360) arg_tedep += edep[i];

      //Identify Carbon Ion
      if(type[i] == 1000060120) carb_tedep += edep[i];

      //Identify Oxygen Ion
      if(type[i] == 1000080160) oxy_tedep += edep[i];

      //Identify Oxygen Ion
      if(type[i] == 1000290630 || type[i] == 1000290650) copper_tedep += edep[i];
    }

  hele->Fill(ele_tedep);
  hprot->Fill(prot_tedep);
  hgamma->Fill(gamma_tedep);
  hneutr->Fill(neutr_tedep);
  harg->Fill(arg_tedep);
  hcarb->Fill(carb_tedep);
  hoxy->Fill(oxy_tedep);
  hcopper->Fill(copper_tedep);
  
  //Info("Process","One entry processed");
     
  return kTRUE;
}

void n_al::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

void n_al::Terminate()
{
  //The Terminate() function is the last function to be called during
  //a query. It always runs on the client, it can be used to present
  //the results graphically or save the results to file.

  cele->cd();
  hele->Draw();

  cprot->cd();
  hprot->Draw();

  cgamma->cd();
  hgamma->Draw();

  cneutr->cd();
  hneutr->Draw();

  // carg->cd();
//   harg->Draw();

//   ccarb->cd();
//   hcarb->Draw();

//   coxy->cd();
//   hoxy->Draw();

//   ccopper->cd();
  //hcopper->Draw();

  Info("Process","One tree processed");

}
