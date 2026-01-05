#include "G4VSensitiveDetector.hh"
/*
#include "LaBr3Hit.hh" 
#include "TrackParticle.hh"
#include "TrackParentParticle.hh"
The previous classes have to be written yet
*/
#include "G4SystemOfUnits.hh"

class DetectorConstruction;
class G4Step;
class G4TouchableHistory;
class RunAction; 
class G4HCofThisEvent; //"Hits Collection of This Event"

class SensitiveDetector : public G4VSensitiveDetector {
    public:
        SensitiveDetector(G4String name);
        ~SensitiveDetector();
    public:
        //Mandatory class methods that must be overloaded
        G4bool ProcessHits(G4Step* step, G4TouchableHistory* R0hist);

        //Optional methods
        void Initialize(G4HCofThisEvent* HCE);
        void EndofEvent(G4HCofThisEvent* HCE);

    
    //private:
    //    typedef std::map<G4 int, LaBr3Hit*> hitMap_t;
        /* "map" is a standard C++ class. Object composed 
        by a douplet of numbers (key, object to which the key referres to)
        "typedef" operation defines a new type of variable called hitMap_t*/
    //    hitMap_t hitMap;

    //    LaBr3HitCollection* HitCollection; //this line is suspicious

    //    typedef std::map<G4int, TrackParticle*> trackMap_t;
    //    trackMap_t trackMap;

    //    typedef std::map<G4int, TrackParentParticle*> trackPArentMap_t;
    //    trackPArentMap_t trackPArentMap;
    
    //public:
    //    trackMap_t GetTrackMap() {
    //        return trackMap;
    //    }

    //ones the missing userdefined libs will be created this has to be uncommented
};