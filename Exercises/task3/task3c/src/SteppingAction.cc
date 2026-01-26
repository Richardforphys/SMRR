// $Id: SteppingAction.cc 94 2010-01-26 13:18:30Z adotti $
/**
 * @file
 * @brief implements user class SteppingAction
 */

#include "SteppingAction.hh"
#include "G4Step.hh"
#include "G4VTouchable.hh"
#include "Analysis.hh"
#include "Randomize.hh"

SteppingAction::SteppingAction()
{}

SteppingAction::~SteppingAction()
{}

void SteppingAction::UserSteppingAction( const G4Step * theStep ) 
{
  // Check energy deposition
  G4double edep = theStep->GetTotalEnergyDeposit();
  if(edep == 0.0) { return; }

  //We need to know if this step is done inside the EM calo or not.
  //We ask the PreStepPoint the volume copy number.
  //Remember: EM calo has copy num 10 or 11
  //We could have asked the volume name, but string
  //comparison is not efficient
  const G4VTouchable* touchable = theStep->GetPreStepPoint()->GetTouchable();
  G4int volCopyNum = touchable->GetVolume()->GetCopyNo();
  if ( volCopyNum == 10 || volCopyNum == 11 ) //EM calo step
    {
      // Find out position along axis Z as a random point 
      // between pre- and post step points.
      // This randomisation allows to smooth histogram profile independently
      // on histogram binning
      G4double z1 = theStep->GetPreStepPoint()->GetPosition().z();
      G4double z2 = theStep->GetPostStepPoint()->GetPosition().z();
      G4double z  = z1 + G4UniformRand()*(z2 - z1);

      // Save energy deposition 
      Analysis::GetInstance()->AddEDepEM( edep, z, volCopyNum );
    }
}

