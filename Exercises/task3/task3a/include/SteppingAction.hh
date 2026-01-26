// $Id: SteppingAction.hh 25 2010-01-05 17:18:05Z adotti $

/**
 * @file   SteppingAction.hh
 *
 * @date   17 Dec 2009
 * @author adotti
 *
 * @brief  User's SteppingAction.
 */

#ifndef SteppingAction_H
#define SteppingAction_H 1

#include "globals.hh"
#include "G4UserSteppingAction.hh"

class SteppingAction : public G4UserSteppingAction {

public:

  SteppingAction();
  virtual ~SteppingAction();

public:

  // The main method to define.
  virtual void UserSteppingAction( const G4Step* );
};

#endif


