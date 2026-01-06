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
        
        virtual G4VPhysicalVolume* Construct(); //Il metodo si deve chiamarare "Construct" e deve restituire un puntatore a G4VPhysicalVolume
        //è il metodo della classe G4UserDetectorConstruction che devo implementare obbligatoriamente
        //! Update Geometry
        void UpdateGeometry(); //metodo per aggiornare la geometria

    private: //sezione privata, accessibile solo dall'interno della classe
        //Define simulation materials
        void DefineMaterials(); //metodo per definire i materiali
        //Initialize geometry parameters
        void ComputeParameters(); //metodo per calcolare i parametri della geometria
        //Construct full geometry of the LaBr3 detector
        G4VPhysicalVolume* Construct_LaBr3Detector(); //metodo per costruire il rilevatore        


    /*================================================== MEMBERS ==================================================*/
    private:
        G4Material* vacuum; //puntatore al materiale vuoto
        G4Material* LaBr3_Mat; //puntatore al materiale del rilevatore LaBr3

    /*================================================== DECLARE LOGICAL VOLUMES ==================================================*/
        //! Global Mother Volume (World)
        G4LogicalVolume* LogicWorld; //puntatore al volume logico del mondo
        //! LaBr3 Detector Volume
        G4LogicalVolume* LogicLaBr3; //puntatore al volume logico del rilevatore LaBr3

    /*================================================== DECLARE PHYSICAL VOLUMES ==================================================*/
        //LaBr3 Detector Physical Volume
        G4VPhysicalVolume* physiLaBr3; //puntatore al volume fisico del rilevatore LaBr3

    /*================================================== DECLARE GEOMETRY PARAMETERS ==================================================*/
        G4double halfWorldLenght; //metà della dimensione del mondo

        G4double RadiusLaBr3Det;
        G4double halfLaBr3Det_z;

        DetectorMessenger* detectorMessenger; //puntatore al messenger del rilevatore
    };