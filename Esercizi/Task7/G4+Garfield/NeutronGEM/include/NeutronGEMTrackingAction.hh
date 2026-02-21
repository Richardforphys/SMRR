#ifndef NeutronGEMTrackingAction_h
#define NeutronGEMTrackingAction_h 1

#include "G4UserTrackingAction.hh"

class NeutronGEMTrackingAction : public G4UserTrackingAction
{
public:
  NeutronGEMTrackingAction();
  virtual ~NeutronGEMTrackingAction(){};

  virtual void PreUserTrackingAction(const G4Track*);
  virtual void PostUserTrackingAction(const G4Track*);

};

#endif
