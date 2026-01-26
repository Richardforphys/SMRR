#include "LaBr3Hit.hh"
#include "G4UnitsTable.hh"

LaBr3Hit::LaBr3Hit(const G4int layer) :
    layerNumber(layer),
    eDep(0) {

}

LaBr3Hit::~LaBr3Hit() {};

void LaBr3Hit::Print() {};