// $Id: EventAction.cc 94 2010-01-26 13:18:30Z adotti $
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
//     #include "SiDigi.hh"
#include "GasHit.hh"
//     #include "SiDigitizer.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4DigiManager.hh"
#include "G4Event.hh"
#include "SensitiveDetector.hh"

EventAction::EventAction()
  : hene(0)
  , rootSaver(0)
  , hitsCollName("GasHitCollection")
  , hitsCollID(-1)

{
}

EventAction::~EventAction()
{
}

void EventAction::AssignHisto(TH1F* histo)
{
  hene = histo;
}

SensitiveDetector* EventAction::GetSensitiveDetector(G4String detname)
{
  
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  
  SensitiveDetector* sens = static_cast<SensitiveDetector*>(SDman->FindSensitiveDetector(detname));
  return sens;
}

void EventAction::BeginOfEventAction(const G4Event* anEvent )
{
  if ( anEvent->GetEventID() % 10 == 0 ) //1000
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

  //G4cout << "End of One EVENT !!!!!!!!!!!!!!!!!!!" << G4endl;

  //G4int layer; 
  //G4double edep; 

  //         //Store information
  //if ( rootSaver )
  //     {
  //Retrieve hits collections
  G4HCofThisEvent* hitsCollections = anEvent->GetHCofThisEvent();
  GasHitCollection* hits = 0;
  if ( hitsCollections )
    {
      hits = static_cast<GasHitCollection*>( hitsCollections->GetHC(hitsCollID) );
    }
  //******WE DO NOT NEED THIS**********************//
  //Get Postion and Momentum of primary
  //This is needed to store in ntuple info @ z=0
  //const G4ThreeVector& pos = anEvent->GetPrimaryVertex()->GetPosition();
  //const G4ThreeVector& mom = anEvent->GetPrimaryVertex()->GetPrimary()->GetMomentum();
  //***********************************************//
  
  if ( hits ) //hits container found we can proceed
    {
      //Loop on all hits
      //Now we loop on hits and add the energy to our arrays,
      //basically we are transforming the hit container in a plain array
      typedef std::vector<GasHit*>::const_iterator hitsVector_it;
      hitsVector_it hit = hits->GetVector()->begin();
      hitsVector_it end = hits->GetVector()->end();
      for ( ; hit != end ; ++hit)
	{
	  layer = (*hit)->GetLayerNumber();
	  edep = (*hit)->GetEdep();
	}
    }
  //G4cout << "Filling histogram >>>>>>>>>>>>>>>>>>>>" << G4endl;
  //G4cout << "Edep = " << edep << G4endl;
  int check = 0;
  if (hene) check = hene->Fill(edep/keV);
  
  //G4cout << "Address of hene: " << static_cast<void*>(hene) << " Check: " 
  //	 << check <<  G4endl;
  
  //*************************************************************//
  //Retrieve the map corrisponfing to SensitiveDetector with sdname

  G4String sdname = "/myDet/ArCO2";
  SensitiveDetector* sensitive = this->GetSensitiveDetector(sdname);

  trackMap_t trackMap = sensitive->GetTrackMap();
  
  //Define iterators
  trackMap_t::iterator itt = trackMap.begin();
  trackMap_t::iterator itt_end = trackMap.end();
  
  // for ( ; itt != itt_end ; ++itt)
//     {
//       G4cout << "\n" ;

//       G4cout << "!!!!!!!!!!!!!!!!EVENT ACTION " 
// 	     << "Energy Deposited by particle ID "
// 	     << itt->first 
// 	     << " that is a " << (itt->second)->GetPart_name() << "  is  " 
// 	     << G4BestUnit( (itt->second)->GetTot_Edep(),"Energy") 
// 	     << G4endl;
      
//     }

  
  
  rootSaver->AddEvent(trackMap, anEvent->GetEventID());

}

