// $Id: DetectorConstruction.hh 33 2010-01-14 17:08:18Z adotti $
/**
 * @file
 * @brief Defines mandatory user class DetectorConstruction.
 */

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4ThreeVector.hh"

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
//class DetectorMessenger;

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
  virtual ~DetectorConstruction();

  //! Construct geometry of the setup
  virtual G4VPhysicalVolume* Construct();

  //! Update geometry
  void UpdateGeometry();

  //! \name some simple set & get functions
  //@{
  G4double FirstSensorPosition()  const { return zFirstSensor; }
  G4double SecondSensorPosition() const { return zSecondSensor; }
  G4double ThirdSensorPosition()  const { return zThirdSensor; }

  G4double SetFirstSensorPosition(G4double z)  { return zFirstSensor=z; }
  G4double SetSecondSensorPosition(G4double z) { return zSecondSensor=z; }
  G4double SetThirdSensorPosition(G4double z)  { return zThirdSensor=z; }

  //@}
private:
  //! define needed materials
  void DefineMaterials();
  //! initialize geometry parameters
  void ComputeParameters();
  //! Construct geometry of the Beam Telescope
  G4VPhysicalVolume* ConstructTelescope();
  //! Construct geometry of the Electromagnetic Calorimeter
  G4VPhysicalVolume* ConstructEMCalo();

private:

  //! \name Materials
  //@{
  G4Material* air;
  G4Material* silicon;
  G4Material* vacuum;
  G4Material* pbw04;
  //@}

  //! \name global mother volume
  //@{
  G4LogicalVolume * logicWorld;
  G4double halfWorldLength;
  G4double gap;
  //@}

  //! \name Geometry tracker
  //@{
  //! 1st telescope plane
  G4VPhysicalVolume* physiFirstSensor;
  //! 2nd telescope plane
  G4VPhysicalVolume* physiSecondSensor;
  //! 3rd telescope plane
  G4VPhysicalVolume* physiThirdSensor;
  //! subdivisions of a plane in sensor strips
  G4VPhysicalVolume * physiSensorStrip; 
  //@}

  //! \name Parameters for tracker
  //@{
  G4int noOfSensorStrips;
  G4double sensorStripLength;
  G4double sensorThickness;
  G4double teleStripPitch;
  G4double zFirstSensor;
  G4double zSecondSensor;
  G4double zThirdSensor;
  //@}

  //! \name Geometry em calo
  //@{
  G4VPhysicalVolume* emCaloCentralCrystal;
  G4VPhysicalVolume* emCalo;
  //@}
  //! \name Parameters for em calo
  //@{
  G4double emCaloLength;
  G4double emCaloWidth;
  G4double emCaloCentralCrystalWidth;
  G4double emCaloZ;
  //@}

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
