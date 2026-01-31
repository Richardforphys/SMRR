#include "G4VSensitiveDetector.hh"
#include "LaBr3Hit.hh"
#include "G4SystemOfUnits.hh"

#include "TrackParticle.hh"
#include "TrackParentParticle.hh"

class DetectorConstruction;
class G4Step;
class RunAction;
class G4TouchableHistory;
class G4HCofThisEvent; //Hit Collection of This Event

class SensitiveDetector : public G4VSensitiveDetector {

    public:
        SensitiveDetector(G4String SDname); //Constructor. Need to give a name to the Sensitive Detector
        ~SensitiveDetector(); //Destructor

        //Mandatory method of base class to implement
        G4bool ProcessHits(G4Step* step, G4TouchableHistory* R0Hist); //Method called at each step in the Sensitive Detector

        //Optional methods of base class to implement
        void Initialize(G4HCofThisEvent* HCE); 
        void EndOfEvent(G4HCofThisEvent* HCE);
    
    private:
        //Mapping layer number to hits
        typedef std::map<G4int, LaBr3Hit*> hitMap_t;
        hitMap_t HitMap;
        
        LaBr3HitCollection* LaBr3HitCollection;

        //Members for user defined classes to keep track of particles
        typedef std::map<G4int, TrackParticle*> trackMap_t;
        trackMap_t trackMap;
        typedef std::map<G4int, TrackParentParticle*> trackParentMap_t;
        trackParentMap_t trackParentMap;

    public:
        //Map getter
        trackMap_t GetTrackMap() { return trackMap; }

}