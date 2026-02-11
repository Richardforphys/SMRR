#ifndef TrackingAction_h
#define TrackingAction_h 1

class G4Track;
#include "G4UserTrackingAction.hh"
#include "globals.hh"

class TrackingAction : public G4UserTrackingAction 
{
  public:
    TrackingAction();
    virtual ~TrackingAction();
   
    virtual void PreUserTrackingAction(const G4Track*);
    virtual void PostUserTrackingAction(const G4Track*);

  private:
    G4String processName;
};

#endif
