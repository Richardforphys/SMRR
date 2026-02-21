#include "StackingAction.hh"
#include "G4ClassificationOfNewTrack.hh"
#include "G4Track.hh"
#include "Analysis.hh"
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
  if ( aTrack->GetParentID() > 0 )//This is a secondary
  {
		Analysis::GetInstance()->AddSecondary(1);
  }

 // G4ParticleDefinition * particleType = aTrack->GetDefinition();
  //GAMMAS
  //if ( particleType == G4Gamma::GammaDefinition() )
  //{
  //}
  return result;

}


