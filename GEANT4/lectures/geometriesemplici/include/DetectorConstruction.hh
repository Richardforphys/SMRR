//Header file, definisco tutti i membri e metodi che implementerò nel file .cc
/*================================================== INCLUDES ==================================================*/

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh" //classe virtuale da cui derivo il detector
#include "G4ThreeVector.hh"              //per definire i vettori 3D
#include "G4SystemOfUnits.hh"          //per usare le unità di misura di Geant4
#include "DetectorMessenger.hh"         
/*================================================== CLASSES ==================================================*/

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material; 
class DetectorMessenger;// questa classe e le precedenti sono incluse all'interno di G4VUserDetectorConstruction.hh

/*Ora costruiamo la classe DetectorConstruction che serve per costruire il rilevatore*/
class DetectorConstruction: public G4VUserDetectorConstruction //deve avere lo stesso nome del file, : vuol dire "derivata da" quello che segue
{ 
    public: 
        DetectorConstruction(); //costruttore
        ~DetectorConstruction(); //distruttore

        //Mandatory methoids to be implemented to construct the geometry of the setup.
}