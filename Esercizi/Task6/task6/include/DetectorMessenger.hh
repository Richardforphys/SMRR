// $Id: DetectorMessenger.hh 33 2010-01-14 17:08:18Z adotti $

#ifndef DetectorMessenger_h
#define DetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class DetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithoutParameter;
class G4UIcmdWithABool;

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
//             
  G4UIcmdWithADoubleAndUnit* dispCmd;    
  G4UIcmdWithoutParameter*   updateCmd;    
};
#endif