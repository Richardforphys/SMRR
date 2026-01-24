//Implementation of the DetectorConstruction class

#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4GeometryTolerance.hh" //Check geometry tolerance
#include "G4GeometryManager.hh" 
#include "G4NistManager.hh" //Nist material manager
#include "G4VisAttributes.hh" //Visualization attributes
#include "G4Colour.hh" 
#include "G4SensitiveDetector.hh"  //To define sensitive detectors
#include "G4SDManager.hh" //To manage sensitive detectors


DetectorConstruction::DetectorConstruction() 
    //Initializer list to set member pointers to null and default values
    :vacuum(0), 
    LaBr3_Mat(0),
    logicWorld(0),
    halfWorldLength(0.5*km)
    {
        //---------------Define Custom UI----------------
        messenger = new DetectorMessenger(this);

        //--------------Material definition----------------
        DefineMaterials();

        //------------Geometry Parameters definition----------------
        ComputeParameters();
    }


DetectorConstruction::~DetectorConstruction() {
    delete messenger;
    delete LaBr3;
}

void DetectorConstruction::DefineMaterials() {
    //Use NIST manager to get predefined materials
    G4NistManager* man = G4NistManager::Instance(); //It's a singleton
    man -> SetVerbose(3); //Set verbosity level (in this case, 3)

    //----------------Define LaBr3 material----------------
    // (maybe there is a predefined one in NIST, but for demonstration, we define it manually)
    //Lantanum
    G4int LanthanumZ = 57;
    G4Element* La = man -> FindOrBuildElement(LanthanumZ);
    if (La) G4cout << "Element Lanthanum found!" << G4endl;

    //Bromine
    G4int BromineZ = 35;
    G4Element* Br = man -> FindOrBuildElement(BromineZ);
    if (Br) G4cout << "Element Bromine found!" << G4endl;

    //LaBr3 material
    G4cout << "Building LaBr3 material" << G4endl;
    G4double LaBr3_density = 5.08 * g/cm3;
    G4int LaBr3_Comp_Atoms = 2;
    G4int n_La_Atoms = 1;
    G4int n_Br_Atoms = 3;
    LaBr3_Mat = new G4Material("LaBr3Mat", LaBr3_density, LaBr3_Comp_Atoms);
    if (!LaBr3_Mat) G4cout << "Error in LaBr3 material definition!" << G4endl;

    //Vacuum material (from Nist)
    vacuum = man -> FindOrBuildMaterial("G4_Galactic");
}    

void DetectorConstruction::ComputeParameters() {
    RadiusLaBr3 = 3.81*cm;
    halfLaBr3_z = 7.762*cm;
}

G4VPhysicalVolume* DetectorConstruction::Construct() { //Method called by the run manager to construct the detector geometry
    //World Solid, Logical Volume and Physical Volume
    G4GeometryManager::GetInstance()->SetWorldMaximumExtent(2*halfWorldLength);
    G4cout << "Computed tolerance = " << G4GeometryTolerance::GetInstance()->GetSurfaceTolerance()/mm << " mm" << G4endl;
    G4Box* solidWorld = new G4Box("World", halfWorldLength, halfWorldLength, halfWorldLength); //Create the world box
    logicWorld = new G4LogicalVolume(solidWorld, //Create the world logical volume
        vacuum, 
        "World", //Its name
        0, //Field Manager
        0, //Sensitive Detector
        0  //User Limits
        0); //Optimization
    G4VPhysicalVolume* physiWorld = new G4PVPlacement(0, //Create the world physical volume, 0=No rotation
        G4ThreeVector(), //At (0,0,0)
        logicWorld, //Its own logical volume
        "World", //Its name
        0, //Its Mother volume, 0 = there is no mother volume
        false, //No boolean operation
        0); //Copy number
    
    //Visualization attributes
    G4Colour
        green(0.0, 1.0, 0.0),
        blue(0.0, 0.0, 1.0),
        red(1.0, 0.0, 0.0),
        white(1.0, 1.0, 1.0),
        brown(0.4, 0.4, 0.1);

    logicWorld -> SetVisAttributes(G4VisAttributes::Invisible());

    Construct_LaBr3Detector();
    return physiWorld; //Always return the world physical volume
}