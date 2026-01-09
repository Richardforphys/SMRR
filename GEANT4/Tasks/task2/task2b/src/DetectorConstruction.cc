/*!
\file
\brief implements mandatory user class DetectorConstruction
*/

#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"


#include "G4GeometryTolerance.hh"
#include "G4GeometryManager.hh"
#include "G4NistManager.hh"

//#include "G4UserLimits.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

//#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
DetectorConstruction::DetectorConstruction()
{
  messenger = new DetectorMessenger(this);

//--------- Material definition ---------
  DefineMaterials();

//--------- Sizes of the principal geometrical components (solids)  ---------
  ComputeParameters();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
DetectorConstruction::~DetectorConstruction()
{
  delete messenger;             
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void DetectorConstruction::DefineMaterials() 
{
  G4NistManager* man = G4NistManager::Instance();
  man->SetVerbose(0);

  // define NIST materials
  air     = man->FindOrBuildMaterial("G4_AIR");
  silicon = man->FindOrBuildMaterial("G4_Si");
  vacuum  = man->FindOrBuildMaterial("G4_Galactic");


  // ********************************************************************************
  // Task1 - Exercise 4 
  //   - change the the sensor planes from silicon to GaAs 
  //     (using the NIST material G4_GALLIUM_ARSENIDE predifined in Geant4)
  //   - define your own material GaAs with the same properties
  // ********************************************************************************

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void DetectorConstruction::ComputeParameters() 
{
  // ** world **
  halfWorldLength = 1.3* m;

  // ** general **
  noOfSensorStrips = 48;
  sensorStripLength = 10.*mm;
  sensorThickness = 300.*um;

  // ** Si beam telescop **
  teleStripPitch  = 20. * um;
  posFirstSensor  = G4ThreeVector(0., 0., 10.*mm);
  posSecondSensor = G4ThreeVector(0., 0., 25.*mm);
  posThirdSensor  = G4ThreeVector(0., 0., 40.*mm);

  // ** Device under test (DUT) **
  dutStripPitch = 50. * um;
  dutTheta = 0.*deg;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
G4VPhysicalVolume* DetectorConstruction::Construct()
{
      
//--------- Definitions of Solids, Logical Volumes, Physical Volumes ---------

  
  //------------------------------ 
  // World
  //------------------------------ 
 
  G4GeometryManager::GetInstance()->SetWorldMaximumExtent(2.*halfWorldLength);
  G4cout << "Computed tolerance = "
         << G4GeometryTolerance::GetInstance()->GetSurfaceTolerance()/mm
         << " mm" << G4endl;


  G4Box * solidWorld= new G4Box("world",halfWorldLength,halfWorldLength,halfWorldLength);
  logicWorld= new G4LogicalVolume( solidWorld, air, "World", 0, 0, 0);
  
  //  Must place the World Physical volume unrotated at (0,0,0).
  // 
  G4VPhysicalVolume * physiWorld = new G4PVPlacement(0,               // no rotation
                                 G4ThreeVector(), // at (0,0,0)
                                 logicWorld,      // its logical volume
				 "World",         // its name
                                 0,               // its mother  volume
                                 false,           // no boolean operations
                                 0);              // copy number
				 



  //
  // 1st Plane of Si Beam Telescope
  //
  G4double halfSensorSizeX = noOfSensorStrips*teleStripPitch/2.;
  G4double halfSensorSizeY = sensorStripLength/2.;
  G4double halfSensorSizeZ = sensorThickness/2.;

  G4Box * solidSensor = new G4Box("Sensor",
			   halfSensorSizeX,halfSensorSizeY,halfSensorSizeZ);

  G4LogicalVolume * logicSensorPlane = new G4LogicalVolume(solidSensor, // its solid
      				   silicon,	//its material
      				   "SensorPlane");	//its name
  physiFirstSensor =
    new G4PVPlacement(0,			//no rotation
		      posFirstSensor,
		      logicSensorPlane,		//its logical volume
		      "FirstSensor",		//its name
		      logicWorld,		//its mother  volume
		      false,			//no boolean operation
		      0);			//copy number


  // 2nd Plane of Si Beam Telescope (to be replaced with DUT)

  // ********************************************************************************
  // Task1 - Exercise 2 
  //   - rotate the central sensor plane by 10 deg around the y-axis.
  // ********************************************************************************
  G4RotationMatrix * rm = new G4RotationMatrix;
  rm->rotateY(dutTheta);

  /*
  physiSecondSensor =
    new G4PVPlacement(rm,
		      posSecondSensor,
		      logicSensorPlane,
		      "SecondSensor",
		      logicWorld,
		      false,
		      1);			//copy number
  */
  ConstructDeviceUnderTest();

  // 3rd Plane of Si Beam Telescope
  // ********************************************************************************
  // Task1 - Exercise 1 
  //   - add an additional third sensor plane at position (0., 0., 1000.*mm)
  // ********************************************************************************
  physiThirdSensor =
    new G4PVPlacement(0,
		      posThirdSensor,
		      logicSensorPlane,
		      "ThirdSensor",
		      logicWorld,
		      false,
		      2);			//copy number




  //
  // Strips
  //
  // ********************************************************************************
  // Task1 - Exercise 3 
  //   - Uncomment and complete the following code to subdivide all sensor planes
  // ********************************************************************************

  G4double halfSensorStripSizeX = teleStripPitch/2.;
  G4double halfSensorStripSizeY = sensorStripLength/2.;
  G4double halfSensorStripSizeZ = sensorThickness/2.;

  G4Box * solidSensorStrip =
    new G4Box("SensorStrip",
	      halfSensorStripSizeX,halfSensorStripSizeY,halfSensorStripSizeZ);

  G4LogicalVolume * logicSensorStrip =
    new G4LogicalVolume(solidSensorStrip,silicon,"SensorStrip");

  physiSensorStrip = new G4PVReplica("SensorStrip",		//its name
				     logicSensorStrip,		//its logical volume
				     logicSensorPlane,		//its mother
				     kXAxis,		        //axis of replication
				     noOfSensorStrips,		//number of replica
				     teleStripPitch);	        //witdth of replica

  G4Color red(1.0,0.0,0.0);
  logicSensorStrip -> SetVisAttributes(new G4VisAttributes(red));



//--------- Visualization attributes -------------------------------

  G4Color 
    yellow(1.0,1.0,0.0),
    green(0.0,1.0,0.0),
    blue(0.0,0.0,1.0),
    brown(0.4,0.4,0.1),
    white(1.0,1.0,1.0);

  logicWorld -> SetVisAttributes(new G4VisAttributes(white));  
  logicWorld -> SetVisAttributes(G4VisAttributes::Invisible);

  logicSensorPlane -> SetVisAttributes(new G4VisAttributes(yellow));
    
  //always return the physical World
  //
  return physiWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::ConstructDeviceUnderTest()
{
  // ********************************************************************************
  // Task1 - Exercise 6 
  //   - Uncomment and complete the following code to create 
  //     a Device Under Test
  // ********************************************************************************

  //
  // Device under Test - replaces 2nd Plane of Si Beam Telescope
  //
  G4double halfSensorSizeX = noOfSensorStrips*dutStripPitch/2.;
  G4double halfSensorSizeY = sensorStripLength/2.;
  G4double halfSensorSizeZ = sensorThickness/2.;
  /*
  G4Box * solidSensor = new G4Box("SensorDUT",
				  ... );

  G4LogicalVolume * logicSensorPlane = new G4LogicalVolume(solidSensor, // its solid
      				   silicon,	//its material
      				   "SensorPlaneDUT");	//its name

  G4RotationMatrix * rm = new G4RotationMatrix;
  rm->rotateY(dutTheta);

  physiSecondSensor =
    new G4PVPlacement( ...,
		      ...,
		      ...,
		      "DeviceUnderTest",
		      ...,
		      false,
		      1);

  //
  // Strips
  //
  
  G4double halfSensorStripSizeX = dutStripPitch/2.;
  G4double halfSensorStripSizeY = sensorStripLength/2.;
  G4double halfSensorStripSizeZ = sensorThickness/2.;

  G4Box * solidSensorStrip = 
    new G4Box("SensorStripDUT",		     
	      ....);

  G4LogicalVolume * logicSensorStrip = 
    new G4LogicalVolume(solidSensorStrip,silicon,"SensorStripDUT");

  physiSensorStripDUT = 
    new G4PVReplica("SensorStripDUT",		//its name
		    ....);	        //witdth of replica

  G4Color red(1.0,0.0,0.0),yellow(1.0,1.0,0.0);
  logicSensorPlane -> SetVisAttributes(new G4VisAttributes(yellow));
  logicSensorStrip -> SetVisAttributes(new G4VisAttributes(red));
  */

  return physiSecondSensor;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4RunManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

void DetectorConstruction::UpdateGeometry()
{
  // Cleanup old geometry
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
