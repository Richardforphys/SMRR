// $Id: StackingAction.cc 94 2010-01-26 13:18:30Z adotti $
/**
 * @file
 * @brief implements user class StackingAction
 */

#include "StackingAction.hh"
#include "G4ClassificationOfNewTrack.hh"
#include "G4Track.hh"
#include "Analysis.hh"

StackingAction::StackingAction()
{}

StackingAction::~StackingAction() 
{}

G4ClassificationOfNewTrack 
StackingAction::ClassifyNewTrack( const G4Track * aTrack ) 
{
  // always "urgent" in current applications
  G4ClassificationOfNewTrack result( fUrgent );

  if ( aTrack->GetParentID() > 0 )//This is a secondary
    {
      Analysis::GetInstance()->AddSecondary(aTrack->GetDefinition());
    }
  else // This is primary
    {
      Analysis::GetInstance()->SetBeam(aTrack->GetDefinition(),
				       aTrack->GetKineticEnergy());
    }

  return result;

}


