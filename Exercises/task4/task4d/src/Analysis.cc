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
//ROOT Stuff
#include "TProfile.h"
#include "TFile.h"

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
	//Create an empty histogram
	//We want to build a profile with energy in each hadronic calorimeter layer,
	//The profile has thus 80 bins in X (one bin per layer)
	//The first bin is 0 and the last one is 79
	hLongProfile = new TProfile("LongitudinalProfile","Hadronic Calo Long Profile",NUMLAYERS,-0.5,NUMLAYERS-0.5);
	hLongProfile->GetXaxis()->SetTitle("Layer Num");
	hLongProfile->GetYaxis()->SetTitle("E_{LAr} (MeV)");
	//Create a histogram for total energy
	hTotE = new TH1F("TotalEnergy","Hadronic Calo total energy in LAr",100,0,500);
	hTotE->GetXaxis()->SetTitle("E_{LAr} (MeV) ");
}

void Analysis::EndOfEvent(const G4Event* anEvent)
{
	//Accumulate over the run
	thisRunTotEM += thisEventTotEM;
	thisRunTotSecondaries += thisEventSecondaries;
	//We want now to retrieve the collection of hits created by the HadCalo SD
	//We need to know the collection ID associated with the collection name,
	// This is the anme of the collection to search
	G4String hadCollName = "HadCaloHitCollection";
	// Get a pointer to the SD (sensitive detector) manager
	G4SDManager* SDman = G4SDManager::GetSDMpointer();
	//Prepare to get an ID of the collection, an integer value
	G4int hitCollID = -1;
	//----------------
	// Exercise 2 of task4d : Step1
	//----------------
	//Retrieve, from the SD manager pointer the ID for hadCollName ("HadCaloHitCollection")
	// Hint: SDman->GetCollectionID( hadCollName ); to get the ID relative to this collection
	// assign this number to hitCollID

	//---------------------
	// Exercise 2 of task4d : Step2
	//----------------
	// Retrieve all collections of hits from the current event (variable anEvent)
	// Hint: use anEvent->GetHCofThisEvent() (HCofThisEvent=Hits Collections of This Event)
	//Retrieve from the current event all collections of all hits
	G4HCofThisEvent* hitsCollections = 0;


	//Now we search the collection that we are interested in
	//From all hits collections get the one we need using the ID
	HadCaloHitCollection* hits = 0;
	if ( hitsCollections )
	{
		//---------------
		// Exercise 2 of task4d : Step3
		//---------------
		//Retrieve the hit collection for Had Calo from hitsCollection
		// Hint 1: retrieve the collection using: hitsCollections->GetHC( ... )
		// Hint 2: remember You need to cast the collection to the proper type
		//         to be able to use it:
		//           hits = (HadCaloHitCollection*) ...put-here-code..........
		//         or alternatively
		//          hits = static_cast<HadCaloHitCollection*>( ...put-here-code...);

	}
	if ( hits ) //hits container found we can proceed
	{
		//Loop on all hits
		//Now we loop on hits and add the energy to our arrays,
		//basically we are transforming the hit container in a plain array
		typedef std::vector<HadCaloHit*>::const_iterator hitsVector_it;
		hitsVector_it hit = hits->GetVector()->begin();
		hitsVector_it end = hits->GetVector()->end();
		for ( ; hit != end ; ++hit)
		{
			G4int layer = (*hit)->GetLayerNumber();
			G4double edep = (*hit)->GetEdep();
			if ( layer >= 0 && layer < NUMLAYERS )
			{
				thisEventTotHad[layer] = edep;
			}
			//else something else very strange happened
			//In this exercise we use simple arrays, this is not the best C++ way to do things, but it's quite simple!
		}
	}

	//-----------------
	// Exercise 3 task4d
	//-----------------
	//Add energy in all layers and fill the hTotE histogram
	// Step 1: declare a variable to summ up all energy in layers
	// Hint : G4double totHadEnergy = 0;
	// Step 2: in the loop below add the energy in each layer to totHadEnergy
	// Step 3: After the loop fill the histogram.
	// Hint : We want to fill the histogram with energy in MeV, you need to convert
	//        totHadEnergy explicitly to MeV!
	//Hint: use hTotE->Fill( ..total_energy.. );

	for ( int i = 0; i<NUMLAYERS ; ++i )
	{
		thisRunTotHad[i] += thisEventTotHad[i];
		//Now fill the profile histogram
		hLongProfile->Fill(i,thisEventTotHad[i]/MeV);

	}

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
	//for ( int layer = 0 ; layer < NUMLAYERS ; ++layer)
	//{
	//	G4cout<<"\t\t Average energy in Layer "<<layer<<": "<<G4BestUnit(thisRunTotHad[layer],"Energy")<<G4endl;
	//}

	//At the end of the run we can now save a ROOT file containing the histogram
	// From the aRun variable we get the run number and use it to create a unique name for the output file
	char filename[256];
	sprintf(filename,"run_%d.root",aRun->GetRunID() );
	TFile* outfile = TFile::Open(filename,"recreate");
	hLongProfile->Write();
	hTotE->Write();
	outfile->Close();
	delete hLongProfile;
	delete hTotE;
	hLongProfile = 0;
	hTotE = 0;
}
