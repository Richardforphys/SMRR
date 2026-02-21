#include "DetectorMessenger.hh"
#include "DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithABool.hh"

DetectorMessenger::DetectorMessenger(DetectorConstruction * det)
  : detector(det)
  , detDir(0)
{ 
      detDir = new G4UIdirectory("/det/");
      detDir->SetGuidance("detector construction commands");

      dispCmd = new G4UIcmdWithADoubleAndUnit("/det/disp",this);
      dispCmd->SetGuidance("Define x-shift of second sensor plane");
      dispCmd->SetParameterName("disp",true);
      dispCmd->SetDefaultValue(1.);
      dispCmd->SetUnitCategory("Length");
      dispCmd->SetDefaultUnit("cm");
    
      updateCmd = new G4UIcmdWithoutParameter("/det/update",this);
      updateCmd->SetGuidance("force to recompute geometry.");
      updateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
      updateCmd->SetGuidance("if you changed geometrical value(s).");
      updateCmd->AvailableForStates(G4State_Idle);

}

DetectorMessenger::~DetectorMessenger()
{
       delete dispCmd;
       delete updateCmd;   
       delete detDir;  
}

void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
       if ( command == dispCmd ) {
         G4double disp = dispCmd->GetNewDoubleValue(newValue);
         detector->SetSecondSensorPosition(disp);
       }

       if ( command == updateCmd )
         detector->UpdateGeometry();
}