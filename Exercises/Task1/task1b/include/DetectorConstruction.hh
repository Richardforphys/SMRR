// $Id: DetectorConstruction.hh 33 2010-01-14 17:08:18Z adotti $
#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

/**
 * @file
 * @brief Defines mandatory user class DetectorConstruction.
 */

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
  ~DetectorConstruction();
public:
  //! Construct geometry of the setup
  G4VPhysicalVolume* Construct();

  //! \name some simple set & get functions
  //@{
  G4ThreeVector FirstSensorPosition() const  { return posFirstSensor; }
  G4ThreeVector SecondSensorPosition() const { return posSecondSensor; }
  G4ThreeVector ThirdSensorPosition() const { return posThirdSensor; }

  G4ThreeVector SetFirstSensorPosition(const G4ThreeVector & pos) { return posFirstSensor=pos; }
  G4ThreeVector SetSecondSensorPosition(const G4ThreeVector & pos) { return posSecondSensor=pos; }
  G4ThreeVector SetThirdSensorPosition(const G4ThreeVector & pos) { return posThirdSensor=pos; }


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
  //! Construct geometry of the hadronic Calorimeter
  G4VPhysicalVolume* ConstructHadCalo();
private:

  //! \name Materials
  //@{
  G4Material* air;
  G4Material* silicon;
  G4Material* vacuum;
  G4Material* pbw04;
  G4Material* fe;
  G4Material* lar;
  //@}

  //! \name global mother volume
  //@{
  G4LogicalVolume * logicWorld;
  G4double halfWorldLength;
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
  G4ThreeVector posFirstSensor;
  G4ThreeVector posSecondSensor;
  G4ThreeVector posThirdSensor;
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
  G4ThreeVector posEmCalo;
  //@}

  //! \name Geometry had calo
  //@{
  G4VPhysicalVolume* hadCalo;
  //@}
  //! \name Parameters for had calo
  //@{
  G4double hadCaloLArThickness;
  G4double hadCaloFeThickness;
  G4double hadCaloRadius;
  G4int    hadCaloNumLayers;
  G4ThreeVector posHadCalo;
  //@}
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
