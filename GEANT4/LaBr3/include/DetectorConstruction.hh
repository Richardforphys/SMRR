// Header file for the DetectorConstruction class

#include    "globals.hh"
#include    "G4VUserDetectorConstruction.hh"
#include    "G4ThreeVector.hh"
#include    "G4SystemOfUnits.hh"
#include    "DetectorMessenger.hh"

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class DetectorMessenger;

class DetectorConstruction : public G4VUserDetectorConstruction  // Define the class DetectorConstruction (same name as the file)
{                                                                // derived publicly from G4VUserDetectorConstruction
    public:
        DetectorConstruction();             // Constructor
        ~DetectorConstruction();            // Destructor

// Mandatory method to construct the detector geometry
        G4VPhysicalVolume* Construct();

};