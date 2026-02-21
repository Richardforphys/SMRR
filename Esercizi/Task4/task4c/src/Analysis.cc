/*
 * Analysis.cc
 *
 *  Created on: 9 Feb 2010
 *      Author: adotti
 */

#include "Analysis.hh"
#include "G4UnitsTable.hh"
#include "G4SDManager.hh"
#include "HadCaloHit.hh"


Analysis* Analysis::singleton = 0;

Analysis::Analysis() :
	thisEventTotEM(0),
	thisEventSecondaries(0),
	thisRunTotEM(0),
	thisRunTotSecondaries(0)
{
}

void Analysis::PrepareNewEvent(const G4Event* /*anEvent*/)
{
	//Reset variables relative to this event
	thisEventSecondaries = 0;
	thisEventTotEM = 0;
	for ( int i = 0; i<NUMLAYERS ; ++i ) thisEventTotHad[i]=0;
}

void Analysis::PrepareNewRun(const G4Run* /*aRun*/ )
{
	//Reset variables relative to the run
	thisRunTotEM = 0;
	thisRunTotSecondaries = 0;
	for ( int i = 0; i<NUMLAYERS ; ++i ) thisRunTotHad[i]=0;
}

void Analysis::EndOfEvent(const G4Event* anEvent)
{
	//Accumulate over the run
	thisRunTotEM += thisEventTotEM;
	thisRunTotSecondaries += thisEventSecondaries;
	//We want now to retrieve the collection of hits created by the HadCalo SD
	//We need to know the collection ID associated with the collection name,
	//for this purpose we can ask the Sensiteive
	G4SDManager* SDman = G4SDManager::GetSDMpointer();
	G4int hitCollID = SDman->GetCollectionID("HadCaloHitCollection");
	G4HCofThisEvent* hitsCollections = anEvent->GetHCofThisEvent();
	HadCaloHitCollection* hits = 0;
	if ( hitsCollections )
	{
		hits = static_cast<HadCaloHitCollection*>(hitsCollections->GetHC(hitCollID));
	}
	if ( hits ) //hits container found we can proceed
	{
		//Loop on all hits
		//Now we loop on hits and add the energy to our conters
		typedef std::vector<HadCaloHit*>::const_iterator hitsVector_it;
		hitsVector_it hit = hits->GetVector()->begin();
		hitsVector_it end = hits->GetVector()->end();
		for ( ; hit != end ; ++hit)
		{
			G4int layer = (*hit)->GetLayerNumber();
			G4double edep = (*hit)->GetEdep();
			if ( layer >= 0 && layer < NUMLAYERS )
			{
				thisEventTotHad[layer] += edep;
			}
			//else something else very strange happened
			//In this exercise we use simple arrays, this is not the best C++ way to do things, but it's quite simple!
		}
	}
	for ( int i = 0; i<NUMLAYERS ; ++i ) thisRunTotHad[i] += thisEventTotHad[i];
}

void Analysis::EndOfRun(const G4Run* aRun)
{
	//Some print outs
	G4int numEvents = aRun->GetNumberOfEvent();

	G4double totHadCalo = 0;
	for ( int i = 0 ; i < NUMLAYERS ; ++i) totHadCalo += thisRunTotHad[i];

	G4cout<<"================="<<G4endl;
	G4cout<<"Summary for run: "<<aRun->GetRunID()<<G4endl;
	G4cout<<"\t Event processed: "<<numEvents<<G4endl;
	G4cout<<"\t Average number of secondaries: "<<thisRunTotSecondaries/numEvents<<G4endl;
	G4cout<<"\t Average energy in EM calo: "<<G4BestUnit(thisRunTotEM/numEvents,"Energy")<<G4endl;
	G4cout<<"\t Average energy in Had calo: "<<G4BestUnit(totHadCalo/numEvents,"Energy")<<G4endl;
	for ( int layer = 0 ; layer < NUMLAYERS ; ++layer)
	{
		G4cout<<"\t\t Average energy in Layer "<<layer<<": "<<G4BestUnit(thisRunTotHad[layer],"Energy")<<G4endl;
	}
}
