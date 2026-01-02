// Deve avere lo stesso nome del file .hh

/*================================================== INCLUDES ==================================================*/
#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"
#include "G4Material.hh"               //per definire i materiali
#include "G4Box.hh"                    //per costruire volumi a forma di scatola
#include "G4Tubs.hh"                   //per costruire volumi a forma di cilindro
#include "G4LogicalVolume.hh"          //per definire i volumi logici
#include "G4PVPlacement.hh"            //per posizionare i volumi fisici
#include "G4GeometryTolerance.hh"      //per la tolleranza geometrica
#include "G4GeometryManager.hh"        //per gestire la geometria
#include "G4NistManager.hh"            //per accedere ai materiali definiti da NIST
#include "G4VisAttributes.hh"          //per definire gli attributi di visualizzazione
#include "G4Colour.hh"                 //per definire i colori

/*
#include "G4SensitiveDetector.hh"   //per i rivelatori sensibili
#include "G4SDManager.hh"          //per gestire i rivelatori sensibili
*/

/*================================================== CONSTRUCTOR ==================================================*/
DetectorConstruction::DetectorConstruction(): 
    vacuum(0), //inizializzo i puntatori a 0
    LaBr3_Mat(0),
    logicWorld(0),
    halfWorldLength(0.5*Km), //inizializzo la metà della dimensione del mondo a 0.5 km
    {
        //Create a detector messenger, defines custom UI commands for this class
        messenger = new DetectorMessenger(this); //this punta all'istanza corrente di DetectorConstruction

        //Define materials
        DefineMaterials();

        //Compute geometry parameters
        ComputeParameters();        
    };

/*================================================== DESTRUCTOR ==================================================*/
DetectorConstruction::~DetectorConstruction()
    {
        delete messenger; //dealloco il messenger del rilevatore
        delete LaBr3_Mat; //dealloco il materiale del rilevatore LaBr3
    }


void DetectorConstruction::DefineMaterials()
    {
        G4NistManager* man = G4NistManager::Instance(); //ottengo l'istanza del gestore NIST
        man->SetVerbose(3); //imposto la verbosità a 3

        //Define Vacuum
        vacuum = man->FindOrBuildMaterial("G4_Galactic"); //definisco il materiale vuoto

        //Define LaBr3 Material
        //LaBr3_Mat = man->FindOrBuildMaterial("G4_LANTHANUM_BROMIDE"); //definisco il materiale del rilevatore LaBr3
        /// Lanthanum
        G4int Lanthanum_Z = 57;
        G4int Lanthanum_A = 138;
        G4Element* L = man->FindOrBuildElement(Lanthanum_Z);
            if (L) {G4cout << "Element Lanthanum found: " << L->GetName() << G4endl;}
            else {G4cout << "Element Lanthanum not found!" << G4endl;}
        /// Bromine
        G4int Bromine_Z = 35;
        G4Element* Br = man->FindOrBuildElement(Bromine_Z);
            if (Br) {G4cout << "Element Bromine found: " << Br->GetName() << G4endl;}
            else {G4cout << "Element Bromine not found!" << G4endl;}
        /// LaBr3 Material
        G4double LaBr3_density = 5.08*g/cm3;
        G4int n_La_Atoms = 1;
        G4int n_Br_Atoms = 3;
        G4int LaBr3_comp_Atoms = 2;
        
        LaBr3_Mat = new G4Material("LaBr3Mat", LaBr3_density, LaBr3_comp_Atoms);

        if(!LaBr3_Mat) 
            { G4cout << "Error in creating LaBr3 Material!" << G4endl;}
    }
/*================================================== DETECTOR CONSTRUCT METHOD ==================================================*/
void DetectorConstruction::ComputeParameters()
    {
        //Compute LaBr3 Detector Cylinder
        RadiusLaBr3Det = 3.81*cm; //raggio del rilevatore LaBr3
        halfLaBr3Det_z = 7.762*cm; //lunghezza del rilevatore LaBr3
    }

G4VPhysicalVolume* DetectorConstruction::Construct()
    {
        // This method is called by G4RunManager to construct the detector
        // Definitions of solids, logical volumes and physical volumes
        
        //--World Volume--//
        G4GeometryManager::GetInstance()->SetWorldMaximumExtent(halfWorldLength*2.); //imposto la massima estensione del mondo
        G4cout << "World maximum extent = " << G4GeometryManager::GetInstance()->GetWorldMaximumExtent()/m << " m" << G4endl;
        G4cout << "Computed Tolerace = " << G4GeometryManager::GetInstance()->GetSurfaceTolerance()/mm << " mm" << G4endl;

        //Define World Solid
        G4Box* solidWorld = new G4Box("World", halfWorldLength, halfWorldLength, halfWorldLength); //creo il solido del mondo
        //Define World Logical Volume
        logicWorld = new G4LogicalVolume(solidWorld, vacuum, "World", 
            0, // Field manager
             0, // Sensitive detector
              0, // User limits
               0 //Optimise
               ); //creo il volume logico del mondo

        //Define World Physical Volume
        G4VPhysicalVolume* physiWorld = new G4PVPlacement(0, G4ThreeVector(0,0,0), logicWorld, "World", 
        0, // its mother volume
        false, // no boolean operation
        0 // copy number
        ); //creo il volume fisico del mondo        

        /*==================================================VISUALIZATION ATTRIBUTES==================================================*/

        G4Color 
            green   (0.0, 1.0, 0.0),
            blue    (0.0, 0.0, 1.0),
            red     (1.0, 0.0, 0.0),
            white   (1.0, 1.0, 1.0),
            brown   (0.6, 0.4, 0.2);

        logicWorld->SetVisAttributes(G4VisAttributes::GetInvisible()); //imposto il mondo come invisibile

        Construct_LaBr3Detector();

        return physiWorld; //Always return Physical world, which is of type G4PhysicalVolume

    }

/*================================================== PHYSICAL VOLUMES ==================================================*/

G4PhysicalVolume* DetectorConstruction::Construct_LaBr3Detector()
{
    G4Color 
            green   (0.0, 1.0, 0.0),
            blue    (0.0, 0.0, 1.0),
            white   (1.0, 1.0, 1.0),
            brown   (0.6, 0.4, 0.2);


    /*Create the LaBr3 detector*/

    G4Tubs* solidLaBr3Det = new G4Tubs("solidLaBr3Det", 0, RadiusLaBr3Det, halfLaBr3Det_z,0*deg,360*deg); //solid

    logicalLaBr3 = new G4LogicalVolume(solidLaBr3Det, LaBr3_Mat, "logical_LaBr3"); //logical

    G4int LaBr3_copynum = 1; //how many?

    physicalLaBr3 = new G4PVPlacement(0, G4ThreeVector(0,0,0), logicalLaBr3, "physi_LaBr3", logicWorld, false, LaBr3_copynum); //physical
    logicalLaBr3->SetVisAttributes(new G4VisAttributes(green));

    


}



