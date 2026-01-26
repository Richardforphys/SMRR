
#include "SimpleField.hh"

SimpleField::SimpleField()
{
  Bvalue = 3.0*kilogauss;        // See units in CLHEP's SystemOfUnits.hh
  // rmax_sq = sqr(50.*cm);
  zMax = -235.*mm;               // Maximum extent in Z of field
}

SimpleField::~SimpleField()
{;}

//  Recall the Z extent of the parts:
// 
//  Si Tracker (three planes) : -500 < Z < -240 mm
//  EM Calo :   -230 < Z < 0 mm  (G4Box)
//  HAD Calo: 0 < Z < 2240 mm (G4Tubs|)
// 

void SimpleField::GetFieldValue(const double Point[3],double *Bfield) const
{
  Bfield[0] = 0.;  // x component
  Bfield[1] = 0.;  // y component
  Bfield[2] = 0.;  // z component

  G4double zCoord= Point[2]; 
  if( (-750.*mm < zCoord) && (zCoord < 175.0*mm) ){
     Bfield[1] = Bvalue; 
  }
}

