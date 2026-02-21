#include "StackingAction.hh"
#include "G4ClassificationOfNewTrack.hh"
#include "G4Track.hh"
#include "Analysis.hh"
#include "G4Gamma.hh"
#include "G4Neutron.hh"
#include "G4SystemOfUnits.hh"
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
  Analysis* analysis = Analysis::GetInstance();
  if ( aTrack->GetParentID() > 0 )//This is a secondary
  {
		analysis->AddSecondary(1);
  }
  G4ParticleDefinition * particleType = aTrack->GetDefinition();
  G4double thresh = 30*MeV;
  if ( particleType == G4Gamma::GammaDefinition() ) {
    //Check that the particle is a Gamma and that its kinetic energy is above threshold:
    //to get track kinetic energy use:
    aTrack->GetKineticEnergy();
    //If the gamma is above threshold count it and kill it, otherwise let it going:
    if (aTrack->GetKineticEnergy() > thresh) {
      analysis->AddGammas(1);
    }
    result = fKill;
    
  } else if (particleType == G4Neutron::NeutronDefinition()) {
    if (aTrack->GetKineticEnergy() > thresh) {
      analysis->AddNeutrons(1);
    }
  }
  return result;
}


