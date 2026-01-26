#include "StackingAction.hh"
#include "G4ClassificationOfNewTrack.hh"
#include "G4Track.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4Neutron.hh"
//#include "G4TrackStatus.hh"
//#include "G4ParticleDefinition.hh"
//#include "G4ParticleTypes.hh"
//#include "G4TouchableHistory.hh"
//#include "G4VPhysicalVolume.hh"
//#include "G4VProcess.hh"
//#include <iostream>
//#include <string>
//#include <cmath>


StackingAction::StackingAction()
{
}


StackingAction::~StackingAction() {
}


G4ClassificationOfNewTrack 
StackingAction::ClassifyNewTrack( const G4Track * aTrack ) {

  G4ClassificationOfNewTrack result( fUrgent );
 
  //This method returns the definition of the partycle type
  G4ParticleDefinition * particleType = aTrack->GetDefinition();

  //Count particles above threshold.
  G4double thresh = 1*MeV;

   if (   particleType == G4Neutron::Neutron() 
       && aTrack->GetKineticEnergy() < thresh 
      ) result = fKill;

   return result;
}


