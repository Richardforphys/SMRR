#define nint_cxx
// The class definition in nint.h has been generated automatically
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
// Root > T->Process("nint.C")
// Root > T->Process("nint.C","some options")
// Root > T->Process("nint.C+")
//

#include "nint.h"
#include <TH2.h>
#include <TStyle.h>
#include <cstdio>

void nint::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

void nint::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

Bool_t nint::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either nint::GetEntry() or TBranch::GetEntry()
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
  //GetEntry(entry);
  
  //Info("Process","Entry number %d processed",entry);
  
  Int_t i = 0;
  Double_t etot=0;
  Int_t tmp[300];
  
  for(int ii = 0; i<300; i++) tmp[ii]=0;
  
  Int_t itmp=0;
  string compos;
  bool mum1 = false;
  
  
  
  //Sum the energy to get the total and save the particles
  //with mumID==1
  for(i=0 ; i <ntracks ; i++)
    {
      etot+= edep[i];
      
      if(mum[i] == 1)
	{
	  mum1 = true;
	  tmp[i] = type[i];
	  itmp++;
	}
      
    }	
  
  //Process this particles in order to create the key for the
  //map
  
  if(mum1)
    {
      for(int j=0; j<itmp; j++)
	{
	  if(tmp[j]==6) cout << "!!!!!!!!!!We found 6 in entry " << entry << endl;
	  if(j!=itmp-1) compos += convertInt(tmp[j]) + "_" ;
	  else compos+= convertInt(tmp[j]);
	}
      
      ProcessMap_t::iterator it = processmap.find(compos);

      TH1F* h;
      
      if ( it != processmap.end() )
	{
	  //This process has been already
	  //found
	  //get it and fill the histogram
	  h = it->second;
	  
	  //std::cout << "Process already exists" << endl;
	  h->Fill(etot);
	}
      else
	{
	  //This process has not yet been found
	  //We have to create it
	  
	  h = new TH1F(compos.c_str(),compos.c_str(),2000,0,2000);
	  
	  processmap.insert( std::make_pair(compos,h) );
	  
	  std::cout << "New process " << compos << " inserted" << endl;

	  h->Fill(etot);
	    
	  
	}
      
      
      //std::cout << "The composed string is: " << compos << endl;
    }


   return kTRUE;
}

void nint::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

void nint::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

  std::cout << "The size of the map is: " << processmap.size() << endl;


}
