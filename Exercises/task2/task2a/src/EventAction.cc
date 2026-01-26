// $Id: EventAction.cc 116 2010-01-29 07:32:37Z schaelic $
/**
 * @file   EventAction.cc
 *
 * @date   17 Dec 2009
 * @author adotti
 * 
 * @brief  Implements user class EventAction.
 */

#include "EventAction.hh"
#include "RootSaver.hh"
#include "SiDigi.hh"
#include "SiHit.hh"
#include "SiDigitizer.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4DigiManager.hh"
#include "G4Event.hh"

EventAction::EventAction() :
	rootSaver(0),
	hitsCollName("SiHitCollection"),
	digitsCollName("SiDigitCollection"),
	hitsCollID(-1)
{
	//We build the digitization module
	SiDigitizer* digitizer = new SiDigitizer("SiDigitizer");
	G4DigiManager * digiManager = G4DigiManager::GetDMpointer();
	digiManager->AddNewModule( digitizer );
}


void EventAction::BeginOfEventAction(const G4Event* anEvent )
{
	if ( anEvent->GetEventID() % 1000 == 0 )
	{
		G4cout<<"Starting Event: "<<anEvent->GetEventID()<<G4endl;
	}
	//Retrieve the ID for the hit collection
	if ( hitsCollID == -1 )
	{
		G4SDManager * SDman = G4SDManager::GetSDMpointer();
		hitsCollID = SDman->GetCollectionID(hitsCollName);
	}
}
void EventAction::EndOfEventAction(const G4Event* anEvent)
{
	//Digitize!!
	G4DigiManager * digiManager = G4DigiManager::GetDMpointer();
	SiDigitizer* digiModule = static_cast<SiDigitizer*>( digiManager->FindDigitizerModule("SiDigitizer") );
	if ( digiModule )
	{
		digiModule->Digitize();
	}

	//Store information
	if ( rootSaver )
	{
		//Retrieve digits collection
		G4int digiCollID = digiManager->GetDigiCollectionID( digitsCollName );
		const SiDigiCollection* digits = static_cast<const SiDigiCollection*>( digiManager->GetDigiCollection(digiCollID) );
		//Retrieve hits collections
		G4HCofThisEvent* hitsCollections = anEvent->GetHCofThisEvent();
		SiHitCollection* hits = 0;
		if ( hitsCollections )
		{
			hits = static_cast<SiHitCollection*>( hitsCollections->GetHC(hitsCollID) );
		}
		//Get Postion and Momentum of primary
		//This is needed to store in ntuple info @ z=0
		const G4ThreeVector& pos = anEvent->GetPrimaryVertex()->GetPosition();
		const G4ThreeVector& mom = anEvent->GetPrimaryVertex()->GetPrimary()->GetMomentum();
		rootSaver->AddEvent(hits,digits,pos,mom);

		hits->PrintAllHits();
	}
}

