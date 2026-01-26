#ifndef EVENT_ACTION_HH
#define EVENT_ACTION_HH

#include "G4UserEventAction.hh"
#include "G4String.hh"
#include "LaBr3Hit.hh"
#include "SensitiveDetector.hh"
#include "G4SystemOfUnits.hh"
#include "TH1F.h"
#include "TFile.h"

class G4Event;
class RootSaver;
class TrackParticle;

class EventAction : public G4UserEventAction {

public:
    EventAction();
    virtual ~EventAction();

    virtual void BeginOfEventAction(const G4Event* anEvent) override;
    virtual void EndOfEventAction(const G4Event* anEvent) override;

    inline void SetRootSaver(RootSaver* saver) { rootSaver = saver; }
    void AssignHisto(TH1F* histo = nullptr);

    SensitiveDetector* GetSensitiveDetector(const G4String& detname);

    // public members
    G4double edep{0.0};
    G4int layer{0};
    TH1F* hene{nullptr};

private:
    RootSaver* rootSaver{nullptr};
    G4String histCollectionName{"LaBr3HitCollection"};
    G4int histCollID{-1};

    typedef std::map<G4int, TrackParticle*> trackMap_t;
};

#endif