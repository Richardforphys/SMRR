// $Id:$
/**
 * @file
 * @brief implements user class StackingAction
 */
#include "StackingAction.hh"
#include "G4ClassificationOfNewTrack.hh"
#include "G4Track.hh"
#include "Analysis.hh"

#include "G4VProcess.hh"
#include "G4ProcessType.hh"

#include "Analysis.hh"

StackingAction::StackingAction()
  : analysis(0)
{
  analysis= Analysis::GetInstance();
}


StackingAction::~StackingAction() {
}


G4ClassificationOfNewTrack 
StackingAction::ClassifyNewTrack( const G4Track * aTrack ) {

  G4ClassificationOfNewTrack result( fUrgent );
  if ( aTrack->GetParentID() > 0 )//This is a secondary
  {
    Analysis::GetInstance()->AddSecondary(1);
    if ( aTrack->GetCreatorProcess()->GetProcessType()==fDecay ) {
      analysis->AddTrack(aTrack);
    }
  }


  return result;







}


