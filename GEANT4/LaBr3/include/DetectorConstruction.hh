//Header file, definisco tutti i membri e metodi che implementerò nel file .cc
/*================================================== INCLUDES ==================================================*/

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh" //classe virtuale da cui derivo il detector
#include "G4ThreeVector.hh"               //per definire i vettori 3D
#include "G4SystemOfUnits.hh"             //per usare le unità di misura di Geant4

/*================================================== CLASSES ==================================================*/
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material; 
class DetectorMessenger; // questa classe e le precedenti sono incluse all'interno di G4VUserDetectorConstruction.hh

/*Ora costruiamo la classe DetectorConstruction che serve per costruire il rilevatore*/
class DetectorConstruction: public G4VUserDetectorConstruction //deve avere lo stesso nome del file, ":" vuol dire "derivata da" quello che segue
{ 
    public: //sezione pubblica, accessibile dall'esterno della classe 
        DetectorConstruction(); //costruttore
        ~DetectorConstruction(); //distruttore
        
        G4VPhysicalVolume* Construct(); //Il metodo si deve chiamarare "Construct" e deve restituire un puntatore a G4VPhysicalVolume
        //è il metodo della classe G4UserDetectorConstruction che devo implementare obbligatoriamente.        

        void UpdateGeometry(); //Non chiaro cosa faccia...
    
    private:

        //----------------------------MEMBERS----------------------------
        //Definisco (non specifico) i membri
        G4Material* vacuum;
        G4Material* LaBr3_Mat;

        //Logical volumes
        G4LogicalVolume* logicWorld;
        G4LogicalVolume* logicLaBr3;

        //Declare Physical Volumes
        G4VPhysicalVolume* physiLaBr3;

        //Declare geometry parameters
        G4double halfWorldLenght;
        G4double RadiusLaBr3Det;
        G4double halfLaBr3Det_z;

        G4ThreeVector DetPosition;
        G4double DetAngle;

        DetectorMessenger* messenger;

        //----------------------------METHODS----------------------------
        //Definisco dei metodi ulteriori che mi permettano di modularizzare la logica esecutiva del programma
        //Un metodo per definire i materiali...uno per la geometria...
        //Define simulation materials
        void DefineMaterials();
        //Initialize geometry parameters
        void ComputeParameters();
        //Construct the full geometry of the LaBr3 detector
        G4VPhysicalVolume* Construct_LaBr3Detector();
        G4VPhysicalVolume* SetPosition(G4ThreeVector pos);
        G4VPhysicalVolume* SetAngle(G4double angle);
}