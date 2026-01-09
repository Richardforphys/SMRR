// $Id: EventAction.hh 25 2010-01-05 17:18:05Z adotti $

/**
 * @file   EventAction.hh
 *
 * @date   17 Dec 2009
 * @author adotti
 *
 * @brief  User's EventAction.
 */

#ifndef EVENTACTION_HH
#define EVENTACTION_HH 1


#include "G4UserEventAction.hh"
#include "G4String.hh"
class G4Event;
/*!
 * \brief User's EventAction class
 * This class implements the Event Action.
 * The relevant method is \sa EndOfEventAction where the
 * digitization (\sa SiDigitizer class) and the storage of hits and digits
 * in a ROOT TTree are perfomed (\sa RootSaver class).
 */
class EventAction : public G4UserEventAction
{
public:
  //! Default constructor
  EventAction();
  //! Default destructor
  virtual ~EventAction();
  //! Beginning of event
  virtual void BeginOfEventAction(const G4Event* anEvent);
  //! Digitize hits and store information
  virtual void EndOfEventAction(const G4Event* anEvent);
};

#endif /* EVENTACTION_HH */
