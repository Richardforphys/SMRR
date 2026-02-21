// $Id: StackingAction.hh 25 2010-01-05 17:18:05Z adotti $

/**
 * @file
 *
 * @date   17 Dec 2009
 * @author adotti
 *
 * @brief  User's StackingAction.
 */
#ifndef STACKINACTION_H
#define STACKINACTION_H 1

#include "globals.hh"
#include "G4UserStackingAction.hh"

class G4Track;
class Analysis;

class StackingAction : public G4UserStackingAction {

public:

  StackingAction();
  virtual ~StackingAction();

  virtual G4ClassificationOfNewTrack ClassifyNewTrack( const G4Track* aTrack );

private:
  Analysis * analysis;
};

#endif

