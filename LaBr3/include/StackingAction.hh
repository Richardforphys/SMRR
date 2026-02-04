#include "globals.hh"
#include "G4UserStackingAction.hh"

class G4Track;

class G4UserStackingAction:: public G4UserStackingAction {

    public:
        StackingAction(); //Constructor
        ~StackingAction(); //Destructor
        
        G4ClassificationOfNewTrack ClassifyNewTrack (const G4Track* aTrack); //Mandatory method
        G4int old_TrackID;
        
}