// $Id: $
/*!
\file
\brief implements class DetectorMessenger
*/

#include "DetectorMessenger.hh"
#include "DetectorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::DetectorMessenger(DetectorConstruction * det)
:detector(det)
{ 
  detDir = new G4UIdirectory("/det/");
  detDir->SetGuidance("detector construction commands");

  secondSensorDir = new G4UIdirectory("/det/secondSensor/");
  secondSensorDir->SetGuidance("comands related to the second sensor plane");


  xShiftCmd = new G4UIcmdWithADoubleAndUnit("/det/secondSensor/xShift",this);
  xShiftCmd->SetGuidance("Define x-shift of second sensor plane");
  xShiftCmd->SetParameterName("xShift",true);
  xShiftCmd->SetDefaultValue(0.);
  xShiftCmd->SetUnitCategory("Length");
  xShiftCmd->SetDefaultUnit("um");

  yShiftCmd = new G4UIcmdWithADoubleAndUnit("/det/secondSensor/yShift",this);
  yShiftCmd->SetGuidance("Define x-shift of second sensor plane");
  yShiftCmd->SetParameterName("yShift",true);
  yShiftCmd->SetDefaultValue(0.);
  yShiftCmd->SetUnitCategory("Length");
  yShiftCmd->SetDefaultUnit("um");

  thetaCmd = new G4UIcmdWithADoubleAndUnit ("/det/secondSensor/theta", this);
  thetaCmd->SetGuidance("Select rotation angle of second sensor plane around y axis");
  thetaCmd->SetParameterName("thetaDUT",true);
  thetaCmd->SetUnitCategory("Angle");
  thetaCmd->SetDefaultUnit("deg");

  updateCmd = new G4UIcmdWithoutParameter("/det/update",this);
  updateCmd->SetGuidance("force to recompute geometry.");
  updateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
  updateCmd->SetGuidance("if you changed geometrical value(s).");
  updateCmd->AvailableForStates(G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::~DetectorMessenger()
{
  delete xShiftCmd;
  delete yShiftCmd;
  delete thetaCmd;

  delete secondSensorDir;

  delete updateCmd;

  delete detDir;  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  if ( command == xShiftCmd ) {
    G4ThreeVector pos= detector->SecondSensorPosition();
    pos.setX( xShiftCmd->GetNewDoubleValue(newValue) );
    detector->SetSecondSensorPosition(pos);
  }

  if ( command == yShiftCmd ) {
    G4ThreeVector pos= detector->SecondSensorPosition();
    pos.setY( yShiftCmd->GetNewDoubleValue(newValue) );
    detector->SetSecondSensorPosition(pos);
  }

  if ( command == thetaCmd )
    detector->SetDUTangle(thetaCmd->GetNewDoubleValue(newValue));
              
  if ( command == updateCmd )
    detector->UpdateGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
