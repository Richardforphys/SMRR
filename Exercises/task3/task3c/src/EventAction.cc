// $Id: EventAction.cc 94 2010-01-26 13:18:30Z adotti $
/**
 * @file
 * @brief implements user class EventAction
 */

#include "EventAction.hh"
#include "G4Event.hh"
#include "Analysis.hh"

EventAction::EventAction()
{}

EventAction::~EventAction()
{}

void EventAction::BeginOfEventAction(const G4Event* anEvent )
{
  if ( anEvent->GetEventID() % 1000 == 0 )
    {
      G4cout<<"Starting Event: "<<anEvent->GetEventID()<<G4endl;
    }
  Analysis::GetInstance()->PrepareNewEvent(anEvent);
}

void EventAction::EndOfEventAction(const G4Event* anEvent)
{
  Analysis::GetInstance()->EndOfEvent(anEvent);
}

