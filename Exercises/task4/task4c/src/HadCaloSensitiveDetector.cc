// $Id: HadCaloSensitiveDetector.cc 100 2010-01-26 16:12:59Z adotti $
/**
 * @file   HadCaloSensitiveDetector.cc
 * @brief  Implements sensitive part of simulation.
 *
 * @date   10 Dec 2009
 * @author Andreas Schaelicke
 */

#include "HadCaloSensitiveDetector.hh"
#include "G4TouchableHistory.hh"
#include "G4Step.hh"
#include "G4HCofThisEvent.hh"
#include "G4HCtable.hh"
#include "G4UnitsTable.hh"
#include "Analysis.hh"

HadCaloSensitiveDetector::HadCaloSensitiveDetector(G4String SDname)
  : G4VSensitiveDetector(SDname)
{
	G4cout<<"Creating SD with name: "<<SDname<<G4endl;
  // 'collectionName' is a protected data member of base class G4VSensitiveDetector.
  // Here we declare the name of the collection we will be using.
	G4String myCollectionName = "HadCaloHitCollection";
	//---------------
	//Exercise 2 task4c
	//---------------
	// Add myCollectionName to the vector of names called collectionName variable
	// Hint1: use insert method:
	//collectionName.insert();
 
  // Note that we may add as many collection names we would wish: ie
  // a sensitive detector can have many collections.
}

HadCaloSensitiveDetector::~HadCaloSensitiveDetector()
{}

G4bool HadCaloSensitiveDetector::ProcessHits(G4Step *step, G4TouchableHistory *)
{
	//This method is called every time a G4Step is performed in the logical volume
	//to which this SD is attached: the HAD calo.

	//To identify where the step is we use the touchable navigation,
	//remember we need to use PreStepPoint!
	G4TouchableHandle touchable = step->GetPreStepPoint()->GetTouchableHandle();
	G4int copyNo = touchable->GetVolume(0)->GetCopyNo();
	//Hadronic layers have number from 1001 to 1080. The index is from 0 to 79:
	G4int layerIndex = copyNo-1001;
	//We get now the energy deposited by this step
	G4double edep = step->GetTotalEnergyDeposit();

	//If the step does not have energy depostion (for example because the particle is
	//a gamma or neutron, then we do not have to do anything
	if ( edep == 0 )
	{
		return true;
	}
	//We will not use anymore directly this method,
	//We show how to get hits from a different place of the codem,
	//See Analaysis class for differences with respect to previous version
	//Analysis::GetInstance()->AddEDepHad(layerIndex,edep);

	//check if edep is from primary or secondary:
	//G4bool isPri = step->GetTrack()->GetTrackID() == 1 ? true : false;
	//G4cout<<"Layer: "<<layerIndex<<" (volume CopyNo: "<<copyNo<<") Edep="<<G4BestUnit(edep,"Energy")<<" isPrimary?"<<isPri;
	//G4cout<<" (name="<<step->GetTrack()->GetDefinition()->GetParticleName()<<")"<<G4endl;

	//Hits in caloriemters are tricky, to avoid to create too many hits we make them
	//accumulate energy on each plane.
	//Thus we need to know if the hit for a specific layer has already been created or not.
	//We use the mapHit map to verify this.
	//We could have done searching the hit in the hitCollection,
	//but a std::map is more efficient in searching.
	//Since this is called several times for each event, this is a place of the code
	//to be optimized
	hitMap_t::iterator it = hitMap.find(layerIndex);
	HadCaloHit* aHit = 0;
	if ( it != hitMap.end() )
	{
		//Hit for this layer already exists
		//remember the hit pointer
		aHit = it->second;
	}
	else
	{
		//Hit for this layer does not exists,
		//we create it
		aHit = new HadCaloHit(layerIndex);
		hitMap.insert( std::make_pair(layerIndex,aHit) );
		hitCollection->insert(aHit);
	}
	aHit->AddEdep( edep );
	return true;
}

void HadCaloSensitiveDetector::Initialize(G4HCofThisEvent* HCE)
{
	// -- Creation of the collection
	// -- collectionName[0] is "SiHitCollection", as declared in constructor

	//---------------
	// Exercise 2 task4c
	//----------------
	// Create the hit collection, remember the hit collection constructor wants two
	// strings: the name of the SD and the name of the collection:
	// hitCollection = new HadCaloHitCollection( EdITME:SD_name, EDITME:collection_name )
	// Hint 1: Get the SD_name with the GetName() function,
	// Hint 2: Get the collection_name from the collectionName vector: your collection is at position 0: collectionName[0]


	// -- and attachment of this collection to the "Hits Collection of this Event":
	// -- To insert the collection, we need to get an index for it. This index
	// -- is unique to the collection. It is provided by the GetCollectionID(...)
	// -- method (which calls what is needed in the kernel to get this index).
	static G4int HCID = -1;
	if (HCID<0) HCID = GetCollectionID(0); // <<-- this is to get an ID for collectionName[0]
	HCE->AddHitsCollection(HCID, hitCollection);

	//Reset map of hits
	hitMap.clear();
}

void HadCaloSensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
	//--------------
	//Exercise 1 of task4c
	//---------------
	//Some verbosity, uncomment this line
	//hitCollection->PrintAllHits();
}
