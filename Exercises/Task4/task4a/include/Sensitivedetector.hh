#include "G4VSensitiveDetector.hh"
using namespace std;

class SensitiveDetector : public G4VSensitiveDetector {
    
    private:
        typedef map<G4int, task4Hit*> hitMap_t;
        

    public:
        SensitiveDetector(G4String SDname);
        ~SensitiveDetector();
        G4bool ProcessHits(G4Step* astep, G4TouchableHistory* R0Hist);

        void Initialize(G4HCofThisEvent* HCE);
        void EndOfEvent(G4HCofThisEvent* HCE);

        trackMap_t GetTrackMap() {return trackMap;}

}