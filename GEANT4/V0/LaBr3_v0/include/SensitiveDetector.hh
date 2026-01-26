#ifndef SENSITIVE_DETECTOR_HH
#define SENSITIVE_DETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include "LaBr3Hit.hh"
#include "G4SystemOfUnits.hh"
#include <map>

class DetectorConstruction;
class G4Step;
class G4TouchableHistory;
class RunAction;
class G4HCofThisEvent;

class SensitiveDetector : public G4VSensitiveDetector {
public:
    SensitiveDetector(const G4String& name);
    ~SensitiveDetector();

    // Mandatory Geant4 method
    G4bool ProcessHits(G4Step* step, G4TouchableHistory* touchable) override;

    // Optional methods
    void Initialize(G4HCofThisEvent* HCE) override;
    void EndOfEvent(G4HCofThisEvent* HCE) override;

    // Uncomment when TrackParticle classes are ready
    
    //typedef std::map<G4int, TrackParticle*> trackMap_t;
    //trackMap_t GetTrackMap() const { return trackMap; }
    

private:
    // Hit collection
    LaBr3HitCollection* hitCollection{nullptr};

    // Hit map (optional)
    // typedef std::map<G4int, LaBr3Hit*> hitMap_t;
    // hitMap_t hitMap;

    // Track maps (optional)
    // trackMap_t trackMap;
    // typedef std::map<G4int, TrackParentParticle*> trackParentMap_t;
    // trackParentMap_t trackParentMap;
};

#endif