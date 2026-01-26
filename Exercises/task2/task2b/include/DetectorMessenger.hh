#ifndef DetectorMessenger_h
#define DetectorMessenger_h 1
// 
// $Id: $
//
/*!
\file
\brief defines class DetectorMessenger
*/

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "globals.hh"
#include "G4UImessenger.hh"

class DetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithoutParameter;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/*!
\brief This class provides the user interface to DetectorConstruction

It allows for
 - change of detector position
 - rotation of the DUT around the y-axis

\sa SetNewValue()
*/
class DetectorMessenger: public G4UImessenger
{
public:
  //! Constructor
  DetectorMessenger(DetectorConstruction* );
  //! Destructor
  ~DetectorMessenger();
    
  //! handle user commands
  void SetNewValue(G4UIcommand*, G4String);
    
private:
  
  DetectorConstruction*      detector;
    
  G4UIdirectory*             detDir;
  G4UIdirectory*             secondSensorDir;
        
  G4UIcmdWithADoubleAndUnit* xShiftCmd;    
  G4UIcmdWithADoubleAndUnit* yShiftCmd;    
  G4UIcmdWithADoubleAndUnit* thetaCmd;    

  G4UIcmdWithoutParameter*   updateCmd;    
};
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

