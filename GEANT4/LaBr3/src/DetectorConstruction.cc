#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4GeometryTolerance.hh" 
#include "G4GeometryManager.hh" //Check della tolleranza geometrica una volta scelta la tolleranza; intersezioni....definizionne corretta della geometria
#include "G4NistManager.hh" //Accesso al database nist x deifnizione materiali
#include "G4VisAttributes.hh" //Gestione della parte di visualizzazione
#include "G4Colour.hh"
#include "SensitiveDetector.hh" //Per estrapolare informazioni da una determinata parte del detector
#include "G4SDManager.hh" //Manager delle parti rese sensitive

DetectorConstruction::DetectorConstruction() :
    : vacuum(0),
    LaBr3_Mat(0),
    logicWorld(0),
    halfWorldLenght(0.5*km)
    {
        //--------------------Crete a messenger--------------------
        messenger = new DetectorMessenger(this);

        //--------------------Material Definition--------------------
        DefineMaterials();

        //--------------------Geometry parameters definition --------------------
        ComputeParameters();    
    }

DetectorConstruction::~DetectorConstruction() {
    delete messenger;
    delete LaBr3;
}

void DetectorConstruction::DefineMaterials() {
    //Get Materials from NIST database
    G4NistManager* man = G4NistManager::Instance(); //E' un singleton!
    man->SetVerbose(3);
    //Define Nist materials
    G4int LanthanumZ = 57;
    G4Element* La = man->FindOrBuildElement(LanthanumZ);
    if(La) G4cout << "Lanthanum correctly retrieved" << G4endl;

    G4int BromineZ = 35;
    G4Element* Br = man->FindOrBuildElement(BromineZ);
    if(Br) G4cout << "Bromine correctly retrieved" << G4endl;

    G4cout << "Building LaBr3 material" << G4endl;
    G4double LaBr3_density = 5.08*g/cm3;

    G4int LaBr3_comp_Atoms =2;
    G4int n_La_Atoms = 1;
    G4int n_Br_Atoms = 3;
    LaBr3_Mat = new G4Material("LaBr3_Mat", LaBr3_density, LaBr3, LaBr3_comp_Atoms);

    if(!LaBr3_Mat) G4cout << "Problems making LaBr3 Material" << G4endl;

    // Retrieving vacuum from nist DB
    vacuum = man -> FindOrBuildMaterial("G4_Galactic");
}

void DetectorConstruction::ComputeParameters() {
    RadiusLaBr3Det = 3.81*cm;
    halfLaBr3Det_z = 7.762*cm;
}

G4VPhysicalVolume* DetectorConstruction::Construct() {
    G4GeometryManager::GetInstance()->SetWorldMaximumExtent(2*halfWorldLenght);
    G4cout << "Computed Tolerance = " << G4GeometryManager::GetInstance()->GetSurfaceTolerance()/mm << " mm" << G4endl;
    G4Box* solidWorld = new G4Box("World", halfWorldLenght, halfWorldLenght, halfWorldLenght);
    logicWorld = new G4LogicalVolume(solidWorld, 
        vacuum, 
        "World", 
        0, // Field Manager
        0, // SensitiveDetector
        0, // UserLimits
        0); // Optimisation
        
    G4VPhysicalVolume* physiWorld = new G4PVPlacement(0, //no rotaion
                                                    G4ThreeVector(0,0,0), //Coordinates
                                                    logicWorld,
                                                    "World",
                                                    0, //Its mother volume!
                                                    false, //no boolean operation
                                                    0); //CopyNumber                                         

    G4Color
        green(0.0, 1.0, 0.0),
        blue(0.0, 0.0, 1.0),
        red(1.0, 1.0, 0.0),
        white(1.0, 1.0, 1.0),
        brown(0.4, 0.4, 0.1);

    logicWorld->SetVisAttributes(G4VisAttributes::Invisible());
    //Call the method to construct all the rest

    Construct_LaBr3Detector();

    return physiWorld;
}  