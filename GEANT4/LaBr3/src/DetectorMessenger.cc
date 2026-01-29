#include "DetectorMessenger.hh"
#include "DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithABool.hh"

DetectorMessenger::DetectorMessenger(DetectorConstruction* det):
    detector(det),
    detDir(0) {
        detDir = new G4UIdirectory("/det/");
        detDir->SetGuidance("Detector Construction Commands");

        xShiftCmd = new G4UicmdWithADoubleAndUnit("/det/XShift", this);
        xShiftCmd->SetGuidance("Define X Shift of detector");
        xShiftCmd->SetParameterName("XShift", true); // True--> Parameter is omittable
        xShiftCmd->SetDefaultValue(0.);
        xShiftCmd->SetUnitCategory("Lenght");
        xShiftCmd->SetDefaultUnit("mm");
        xShiftCmd->AvailableForStates(G4State_Idle);

        yShiftCmd = new G4UicmdWithADoubleAndUnit("/det/XShift", this);
        yShiftCmd->SetGuidance("Define Y Shift of detector");
        yShiftCmd->SetParameterName("YShift", true); 
        yShiftCmd->SetDefaultValue(0.);
        yShiftCmd->SetUnitCategory("Lenght");
        yShiftCmd->SetDefaultUnit("mm");
        yShiftCmd->AvailableForStates(G4State_Idle);

        zShiftCmd = new G4UicmdWithADoubleAndUnit("/det/XShift", this);
        zShiftCmd->SetGuidance("Define Z Shift of detector");
        zShiftCmd->SetParameterName("ZShift", true); 
        zShiftCmd->SetDefaultValue(0.);
        zShiftCmd->SetUnitCategory("Lenght");
        zShiftCmd->SetDefaultUnit("mm");
        zShiftCmd->AvailableForStates(G4State_Idle);

        thetaCmd = new G4UicmdWithADoubleAndUnit("/det/theta", this);
        thetaCmd->SetGuidance("Define rotation angle of the detector");
        thetaCmd->SetParameterName("Theta", true); 
        thetaCmd->SetDefaultValue(0.);
        thetaCmd->SetUnitCategory("Angle");
        thetaCmd->SetDefaultUnit("deg");
        thetaCmd->AvailableForStates(G4State_Idle);

        updateCmd = new G4UIcmdWithoutParameter("/det/update", this);
        updateCmd->SetGuidance("Update Geometry. Must be applied before /run/beamOn");
        updateCmd->AvailableForStates(G4State_Idle);

}

DetectorMessenger::~DetectorMessenger() {
    delete xShiftCmd;
    delete yShiftCmd;
    delete zShiftCmd;
    delete thetaCmd;
    delete updateCmd;
}

void DetectorMessenger::SetNewValue(G4UICommand* command, G4string newValue) {
    
    if (command == xShiftCmd) {
        G4ThreeVector pos = detector->DetPosition;
        pos.SetX(xShiftCmd->GetNewDoubleValue(newValue));
        detector->SetPosition(pos);
    }

    if (command == yShiftCmd) {
        G4ThreeVector pos = detector->DetPosition;
        pos.SetY(yShiftCmd->GetNewDoubleValue(newValue));
        detector->SetPosition(pos);
    }

    if (command == zShiftCmd) {
        G4ThreeVector pos = detector->DetPosition;
        pos.SetZ(yShiftCmd->GetNewDoubleValue(newValue));
        detector->SetPosition(pos);
    }

    if (command == thetaCmd) {
        G4double angle = detector->DetAngle;
        detector->SetAngle(angle);
    }

    if (command == updateCmd) {
        detector->UpdateGeometry();
    }

}