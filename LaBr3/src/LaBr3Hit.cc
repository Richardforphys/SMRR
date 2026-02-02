#include "LaBr3Hit.hh"
#include "G4SystemOfUnits.hh"

LaBr3Hit::LaBr3Hit(const G4int layer) 
    layerNumber (layer) ,
    eDep(0){}

LaBr3Hit::~LaBr3Hit() {}

void LaBr3Hit::Print() {
    G4double eDep = this->GetEdep();
    G4int layerN = this->GetLayerNumber();
    G4cout << "HIT : Deposited Energy is " << G4BestUnit(eDep, Energy)
    << " in layer " << layer << G4endl;
}