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
  //collectionName.insert("SiHitCollection");
 
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

	Analysis::GetInstance()->AddEDepHad(layerIndex,edep);

	//check if edep is from primary or secondary:
	//G4bool isPri = step->GetTrack()->GetTrackID() == 1 ? true : false;
	//G4cout<<"Layer: "<<layerIndex<<" (volume CopyNo: "<<copyNo<<") Edep="<<G4BestUnit(edep,"Energy")<<" isPrimary?"<<isPri;
	//G4cout<<" (name="<<step->GetTrack()->GetDefinition()->GetParticleName()<<")"<<G4endl;
	return true;
}

void HadCaloSensitiveDetector::Initialize(G4HCofThisEvent* /*HCE*/)
{
}

void HadCaloSensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
}
