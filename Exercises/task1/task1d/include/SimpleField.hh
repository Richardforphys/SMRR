#ifndef SimpleField_H
#define SimpleField_H 1

#include "globals.hh"
#include "G4MagneticField.hh"

class SimpleField : public G4MagneticField
{
  public:
    SimpleField();
    ~SimpleField();

    void GetFieldValue( const  double Point[3],
                               double *Bfield ) const;

  private:
    G4double Bvalue;
  // G4double rmax_sq;
  // G4double zMin;
    G4double zMax;
};

#endif

