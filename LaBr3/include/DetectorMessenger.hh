/*---------------------------------------------------------------------------------------------------------------------------------------
    Construct class for handling User Interface commands
    Reminder : GUI(write command, enter) --> Call UIManager instance --> Call UiMessenger for specific class (geometry, primaries...)
    --> Convert command string into call for a specific method of the target class
---------------------------------------------------------------------------------------------------------------------------------------*/

#include "G4UIMessenger.hh"
#include "globals.hh"
#include "DetectorConstruction.hh" //Target class

class G4UIDirectory;
class G4UicmdWithAString;
class G4UicmdWithADoubleAndUnit;
class G4UicmdWith3VectorAndUnit;
class G4UicmdWithoutParameter;
class G4UicmdWithAString;
class G4UicmdWithABool;

class DetectorMessenger:: public G4UIMessenger  {

    public:
        DetectorMessenger(DetectorConstruction*);
        ~DetectorMessenger();

        void SetNewValue(G4UICommand*, G4string);

    private:
        DetectorConstruction* detector;
        G4UIdirectory* detDir; //directory for UI detector command 
        G4UicmdWithADoubleAndUnit* xShiftCmd;
        G4UicmdWithADoubleAndUnit* yShiftCmd;
        G4UicmdWithADoubleAndUnit* zShiftCmd;
        G4UicmdWithADoubleAndUnit* thetaCmd; //Pointer to commands to shift geometry?
        void SetPosition (G4ThreeVector* pos) {
            DetPosition = pos;
        }
        G4UicmdWithoutParameter* updateCmd;
}