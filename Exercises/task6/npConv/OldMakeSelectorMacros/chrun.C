#define chrun_cxx
// The class definition in chrun.h has been generated automatically
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
// Root > T->Process("chrun.C")
// Root > T->Process("chrun.C","some options")
// Root > T->Process("chrun.C+")
//

#include "chrun.h"
#include <TH2.h>
#include <TStyle.h>


void chrun::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();


   hele = new TH1F("ElectronEnDep","ElectronEnDep",500,0,500);
   hprot = new TH1F("ProtonEnDep","ProtonEnDep",1000,0,1000);
   hgamma = new TH1F("GammaEnDep","GammaEnDep",100,0,100);
   hneutr = new TH1F("NeutronEnDep","NeutronEnDep",100,0,100);
   harg = new TH1F("ArgonEnDep","ArgonEnDep",550,0,550);
   hcarb = new TH1F("CarbonEnDep","CarbonEnDep",1000,0,1000);
   hoxy = new TH1F("OxygenEnDep","OxygenEnDep",1000,0,1000);
   hcopper = new TH1F("CopperEnDep","CopperEnDep",500,0,500);
   htot = new TH1F("TotEnDep","TotEnDep",1000,0,1000);
   hbump = new TH1F("BumpEnDep","BumpEnDep",1000,0,1000);
   heleprot = new TH1F("ElProtEnDep","ElProtEnDep",500,0,500);
   helearg = new TH1F("ElArgEnDep","ElArgEnDep",500,0,500);
   hprotarg = new TH1F("ProtArgEnDep","ProtArgEnDep",500,0,500);


   cele = new TCanvas("CanvEl","CanvEl");
   cprot = new TCanvas("CanvPr","CanvPr");
   cgamma = new TCanvas("CanvGam","CanvGam");
   cneutr = new TCanvas("CanvNeut","CanvNeutr");
   carg = new TCanvas("CanvArg","CanvArg");
   ccarb = new TCanvas("CanvCarb","CanvCarb");
   coxy = new TCanvas("CanvOxy","CanvOxy");
   ccopper = new TCanvas("CanvCop","CanvCop");
   ctot = new TCanvas("CanTot","CanTot");
   cbump = new TCanvas("CanBump","CanBump");
   celeprot = new TCanvas("CanvElProt","CanvElProt");
   celearg = new TCanvas("CanvElArg","CanvElArg");
   cprotarg = new TCanvas("CanvProtArg","CanvProtArg");


   Info("Begin","Histos and Canvas declared");


}

void chrun::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

Bool_t chrun::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either chrun::GetEntry() or TBranch::GetEntry()
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
  
  Info("Process","Entry number %d processed",entry);

  Double_t ele_tedep = 0;
  Double_t prot_tedep = 0 ;
  Double_t gamma_tedep = 0;
  Double_t neutr_tedep = 0;
  Double_t arg_tedep = 0;
  Double_t carb_tedep = 0;
  Double_t oxy_tedep = 0;
  Double_t copper_tedep = 0;
  Double_t tot_edep = 0;
  Double_t bump_tedep = 0;
  Double_t eleprot_tedep = 0;
  Double_t elearg_tedep = 0;
  Double_t protarg_tedep = 0;

  
   for(Int_t i=0; i <= ntracks; i++)
    {
      //Identify Electrons
      
      tot_edep+=edep[i];

      if( type[i] == 11 ) ele_tedep += edep[i];

      //Identify Protons
      if(type[i] == 2212) prot_tedep += edep[i];
      
      //Identify Neutrons
      if(type[i] == 2121) neutr_tedep += edep[i];
      //
      //Identify Gamma
      if(type[i] == 22) gamma_tedep += edep[i];
      //
      //Identify Argon Ion
      if( (type[i] == 1000180400) || (type[i] == 1000180360) ) arg_tedep += edep[i];
      //
      //Identify Carbon Ion
      if(type[i] == 1000060120) carb_tedep += edep[i];
      //
      ////Identify Oxygen Ion
      if( type[i] == 1000080160) oxy_tedep += edep[i];
      //
      //Identify Copper Ion
      if( (type[i] == 1000290630) || (type[i] == 1000290650) ) copper_tedep += edep[i];
      //
      //Try to sum electrons, protons and Ar Ions in the bump
      if( (type[i] == 11)         || 
	  (type[i] == 2212)       || 
	  (type[i] == 1000180400) || 
	  (type[i] == 1000180360) ) bump_tedep += edep[i];
      //
      //Try to sum only electrons and protons
      if( (type[i] == 11) || 
	  (type[i] == 2212) ) eleprot_tedep += edep[i];
      //
      //Try to sum only electrons and argon
      if( (type[i] == 11)         ||  
	  (type[i] == 1000180400) || 
	  (type[i] == 1000180360) ) elearg_tedep += edep[i];
      //
      //Try to sum only protons and argon
      //Try to sum only electrons and argon
      if( (type[i] == 2212)       ||  
	  (type[i] == 1000180400) || 
	  (type[i] == 1000180360) ) protarg_tedep += edep[i];
    
    }


   //Info("Process","Edep = %f",ele_tedep);
  
   hele->Fill(ele_tedep);
   hprot->Fill(prot_tedep);
   hgamma->Fill(gamma_tedep);
   hneutr->Fill(neutr_tedep);
   harg->Fill(arg_tedep);
   hcarb->Fill(carb_tedep);
   hoxy->Fill(oxy_tedep);
   hcopper->Fill(copper_tedep);
   htot->Fill(tot_edep);
   hbump->Fill(bump_tedep);
   heleprot->Fill(eleprot_tedep);
   helearg->Fill(elearg_tedep);
   hprotarg->Fill(protarg_tedep);;


   return kTRUE;
}

void chrun::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

void chrun::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

  cele->cd();
  hele->Draw();

  cprot->cd();
  hprot->Draw();

  cgamma->cd();
  hgamma->Draw();
  //
  cneutr->cd();
  hneutr->Draw();
  //
  carg->cd();
  harg->Draw();
  //
  ccarb->cd();
  hcarb->Draw();
  //
  coxy->cd();
  hoxy->Draw();
  //
  ccopper->cd();
  hcopper->Draw();

  ctot->cd();
  htot->Draw();

  hele->SetLineColor(2);
  hprot->SetLineColor(3);

  //hele->Draw("same");
  //hprot->Draw("same");

  cbump->cd();
  hbump->Draw();

  ctot->cd();
  //hbump->SetLineColor(4);
  hbump->Draw("same");

  celeprot->cd(); 
  heleprot->SetLineColor(2);
  heleprot->Draw();


  celearg->cd();
  helearg->SetLineColor(3);
  helearg->Draw();


  cprotarg->cd();
  hprotarg->SetLineColor(4);
  hprotarg->Draw();
    
  
  ctot->cd();
  heleprot->Draw("same");
  helearg->Draw("same");
  hprotarg->Draw("same");

}
