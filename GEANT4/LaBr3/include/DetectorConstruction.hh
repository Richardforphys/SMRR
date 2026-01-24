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
// derived publicly from G4VUserDetectorConstruction
{                                                                
    public:

        DetectorConstruction();             // Constructor
        ~DetectorConstruction();            // Destructor

//-------------------------------------------METHODS----------------------------------------

        // Mandatory method to construct the detector geometry
        G4VPhysicalVolume* Construct();    // Override the Construct method from G4VUserDetectorConstruction, returns a pointer to the world physical volume

        void UpdateGeometry();              // Method to update the geometry when parameters change

    private:

        void DefineMaterials();            // Method to define materials used in the detector 
        void ComputeParameters();          // Method to compute parameters based on user input
        
        G4VPhysicalVolume* Construct_LaBr3Detector(); // User-defined method to construct the LaBr3 detector geometry

//-------------------------------------------MEMBERS---------------------------------------- 

        private:
        G4Material* vacuum;
        G4Material* LaBr3_Mat;

        //Declare Logical Volumes
        G4LogicalVolume* logicWorld; //Global Mother Volume
        G4LogicalVolume* logicLaBr3; //LaBr3 detector logical volume

        //Declare Physical Volumes
        G4VPhysicalVolume* physiLaBr3; //LaBr3 detector physical volume
        
        //Declare Geometry parameters
        G4double halfWorldLength; 
        G4double RadiusLaBr3;
        G4double halfLaBr3_z;

        DetectorMessenger* detectorMessenger; // Pointer to the DetectorMessenger for user commands


};