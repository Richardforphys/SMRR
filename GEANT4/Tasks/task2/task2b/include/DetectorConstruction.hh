#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

/*!
\file
\brief defines mandatory user class DetectorConstruction
*/

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4ThreeVector.hh"

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class DetectorMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/*!
\brief This mandatory user class defines the geometry.

It is responsible for
 - Definition of material, and
 - Construction of geometry

\sa Construct()
 */
class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
  //! Constructor
  DetectorConstruction();
  //! Destructor
  ~DetectorConstruction();
public:
  //! Construct geometry of the setup
  G4VPhysicalVolume* Construct();

  //! Update geometry
  void UpdateGeometry();

  //! Construct device under test
  G4VPhysicalVolume* ConstructDeviceUnderTest();

  //! \name some simple set & get functions
  //@{
  G4ThreeVector FirstSensorPosition() const  { return posFirstSensor; }
  G4ThreeVector SecondSensorPosition() const { return posSecondSensor; }
  G4ThreeVector ThirdSensorPosition() const { return posThirdSensor; }

  G4ThreeVector SetFirstSensorPosition(const G4ThreeVector & pos) { return posFirstSensor=pos; }
  G4ThreeVector SetSecondSensorPosition(const G4ThreeVector & pos) { return posSecondSensor=pos; }
  G4ThreeVector SetThirdSensorPosition(const G4ThreeVector & pos) { return posThirdSensor=pos; }

  G4double DUTangle() const { return dutTheta; }
  G4double SetDUTangle(const G4double theta)  { return dutTheta=theta; }
  //@}
private:
  //! define needed materials
  void DefineMaterials();
  //! initialize geometry parameters
  void ComputeParameters();

private:

  //! \name Materials
  //@{
  G4Material* air;
  G4Material* silicon;
  G4Material* vacuum;
  //@}

  //! \name Geometry
  //@{

  //! global mother volume
  G4LogicalVolume * logicWorld;

  //! 1st telescope plane
  G4VPhysicalVolume* physiFirstSensor;
  //! 2nd telescope plane
  G4VPhysicalVolume* physiSecondSensor;
  //! 3rd telescope plane
  G4VPhysicalVolume* physiThirdSensor;

  //! subdivisions of a plane in sensor strips
  G4VPhysicalVolume * physiSensorStrip; 
  //! subdivisions of the DUT in sensor strips
  G4VPhysicalVolume * physiSensorStripDUT; 
  //@}

  //! \name Parameters
  //@{
  G4double halfWorldLength;

  G4int noOfSensorStrips;
  G4double sensorStripLength;
  G4double sensorThickness;

  G4double teleStripPitch;
  G4ThreeVector posFirstSensor;
  G4ThreeVector posSecondSensor;
  G4ThreeVector posThirdSensor;

  G4double dutStripPitch;
  G4double dutTheta;
  //@}

  //! \name UI Messenger 
  //@{
  DetectorMessenger * messenger;
  //@}
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
