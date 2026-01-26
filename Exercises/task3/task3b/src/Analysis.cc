/*
 * Analysis.cc
 *
 *  Created on: 9 Feb 2010
 *      Author: adotti
 */
/**
 * @file
 * @brief implements singleton class Analysis 
 */

#include "Analysis.hh"
#include "G4UnitsTable.hh"
#include "G4Track.hh"

#include "TH1D.h"
#include "TFile.h"

Analysis* Analysis::singleton = 0;

Analysis::Analysis() :
	thisEventTotEM(0),
	thisEventSecondaries(0),
	thisRunTotEM(0),
	thisRunTotSecondaries(0)
{
}

Analysis::~Analysis() 
{}

void Analysis::AddTrack( const G4Track * aTrack ) 
{
  if (histos.size()>3) {
    if (aTrack->GetDefinition()->GetPDGEncoding()!=11) return; // electrons

    const G4ThreeVector & pos = aTrack->GetPosition();
    const G4ThreeVector & mom = aTrack->GetMomentumDirection();
    G4double time = aTrack->GetGlobalTime();


    histos[fDecayPosZ]->Fill(pos.z()/m);
    histos[fDecayTime]->Fill(time/millisecond);
    if (mom.z()>0) histos[fDecayTimeForward]->Fill(time/millisecond);
    else histos[fDecayTimeBackward]->Fill(time/millisecond);
  }
}

void Analysis::PrepareNewEvent(const G4Event* /*anEvent*/)
{
	//Reset variables relative to this event
	thisEventSecondaries = 0;
	thisEventTotEM = 0;
}

void Analysis::PrepareNewRun(const G4Run* /*aRun*/ )
{
	//Reset variables relative to the run
	thisRunTotEM = 0;
	thisRunTotSecondaries = 0;

	TH1D *h=0;
	// create Histograms
	histos.push_back(h=new TH1D("decayPos","Z Position of Decay",100,0.8*m,(0.8+2.24)*m) );
	h->GetYaxis()->SetTitle("events");
	h->GetXaxis()->SetTitle("t_{decay} #mus");
	h->StatOverflows();
	histos.push_back(h=new TH1D("decayTime","Time of Decay",200,0,20 ) ); //microsecond
	h->GetYaxis()->SetTitle("events");
	h->GetXaxis()->SetTitle("t_{decay} #mus");
	h->StatOverflows();
	histos.push_back(h=new TH1D("decayTimeForward","Time of Decay [Forward electron]",200,0,20) ); // microsecond
	h->GetYaxis()->SetTitle("forward events");
	h->GetXaxis()->SetTitle("t_{decay} #mus");
	h->StatOverflows();
	histos.push_back(h=new TH1D("decayTimeBackward","Time of Decay [Backward electron]",200,0,20) ); // microsecond
	h->GetYaxis()->SetTitle("backward events");
	h->GetXaxis()->SetTitle("t_{decay} #mus");
	h->StatOverflows();
}

void Analysis::EndOfEvent(const G4Event* /*anEvent*/)
{
	//Accumulate over the run
	thisRunTotEM += thisEventTotEM;
	thisRunTotSecondaries += thisEventSecondaries;
	//Uncomment this line for more verbosity:
	//	G4cout<<"Event: "<<anEvent->GetEventID() <<" Energy in EM calo: "<<G4BestUnit(thisEventTotEM,"Energy")<<" Secondaries: "<<thisEventSecondaries<<G4endl;
}

void Analysis::EndOfRun(const G4Run* aRun)
{
	//Some print outs
	G4int numEvents = aRun->GetNumberOfEvent();

	G4cout<<"================="<<G4endl;
	G4cout<<"Summary for run: "<<aRun->GetRunID()<<G4endl;
	G4cout<<"\t Event processed: "<<numEvents<<G4endl;
	G4cout<<"\t Average number of secondaries: "<<thisRunTotSecondaries/numEvents<<G4endl;
	G4cout<<"\t Average energy in EM calo: "<<G4BestUnit(thisRunTotEM/numEvents,"Energy")<<G4endl;
	G4cout<<"================="<<G4endl;

	//At the end of the run we can now save a ROOT file containing the histogram
	char filename[256];
	sprintf(filename,"run_%d.root",aRun->GetRunID() );
	TFile* outfile = TFile::Open(filename,"recreate");
	for (size_t i=0; i<histos.size();++i) 
	  histos[i]->Write();
	outfile->Close();
	// delete histos
	for (size_t i=0; i<histos.size();++i) 
	  delete histos[i];
	histos.clear();
}
