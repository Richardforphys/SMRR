// $Id: DetectorConstruction.cc 94 2010-01-26 13:18:30Z adotti $
/**
 * @file
 * @brief Implements mandatory user class DetectorConstruction.
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

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "SensitiveDetector.hh"
#include "G4SDManager.hh"

DetectorConstruction::DetectorConstruction()
{
	//Create a messanger (defines custom UI commands)
	messenger = new DetectorMessenger(this);

	//--------- Material definition ---------
	DefineMaterials();

	//--------- Sizes of the principal geometrical components (solids)  ---------
	ComputeParameters();
}
 
DetectorConstruction::~DetectorConstruction()
{
  delete messenger;             
}
 
void DetectorConstruction::DefineMaterials() 
{
	//Get Materials from NIST database
	G4NistManager* man = G4NistManager::Instance();
	man->SetVerbose(0);

	// define NIST materials
	air     = man->FindOrBuildMaterial("G4_AIR");
	silicon = man->FindOrBuildMaterial("G4_Si");
	vacuum  = man->FindOrBuildMaterial("G4_Galactic");
}
 
void DetectorConstruction::ComputeParameters() 
{
	//This function defines the defaults
	//of the geometry construction

	// ** world **
	halfWorldLength = 1.3* m;

	// ** general **
	noOfSensorStrips = 48;
	sensorStripLength = 10.*mm;
	sensorThickness = 300.*um;

	// ** Si beam telescop **
	teleStripPitch  = 20. * um;
	posFirstSensor  = G4ThreeVector(0., 0.,  200.*mm);
	posSecondSensor = G4ThreeVector(0., 0.,  600.*mm);
	posThirdSensor  = G4ThreeVector(0., 0., 1000.*mm);

	// ** Device under test (DUT) **
	isSecondPlaneDUT = false; //By default construct a SiTelescope
	dutStripPitch = 50. * um;
	dutTheta = 0.*deg;
}
 
G4VPhysicalVolume* DetectorConstruction::Construct()
{
	//This function is called by G4 when the detector has to be created
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
				 


	//The construction of the three si plane is actually done here
	ConstructTelescope();


	//--------- Visualization attributes -------------------------------

	G4Color
	  green(0.0,1.0,0.0),
	  blue(0.0,0.0,1.0),
	  brown(0.4,0.4,0.1),
	  white(1.0,1.0,1.0);
	
	logicWorld -> SetVisAttributes(new G4VisAttributes(white));
	logicWorld -> SetVisAttributes(G4VisAttributes::Invisible);
	
	
	//  PrintTelescopeParameters();
	
	
	//always return the physical World
	//
	return physiWorld;
}

G4VPhysicalVolume* DetectorConstruction::ConstructTelescope()
{
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

	physiFirstSensor = new G4PVPlacement(0,	//no rotation
				  posFirstSensor,
				  logicSensorPlane,		//its logical volume
				  "FirstSensor",		//its name
				  logicWorld,		//its mother  volume
				  false,			//no boolean operation
				  0);			//copy number


	// 2nd Plane of Si Beam Telescope (to be replaced with DUT)
	if ( isSecondPlaneDUT )
	{
	  G4cout<<"Building Device Under Test setup: second plane is replaced by DUT"<<G4endl;
	  ConstructDeviceUnderTest();
	}
	else
	{
		G4RotationMatrix * rm = new G4RotationMatrix;
		rm->rotateY(dutTheta);

		physiSecondSensor = new G4PVPlacement(rm,
						  posSecondSensor,
						  logicSensorPlane,
						  "SecondSensor",
						  logicWorld,
						  false,
						  1);			//copy number
	}

	// 3rd Plane of Si Beam Telescope
	physiThirdSensor = new G4PVPlacement(0,
				  posThirdSensor,
				  logicSensorPlane,
				  "ThirdSensor",
				  logicWorld,
				  false,
				  2);			//copy number

	//
	// Strips
	//
	//Note that we use G4VReplica giving as logical volume
	//the logicSensorPlane as mother. This ensures that three
	//replicas will exists, one for each physical volume where
	//logicSensorPlane is used: the Si planes
	//In case of using DUT the thing is a bit different.
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



	// ----------------------------------------------------------
	// -- Binding SensitiveDetector code to astronaut volume:
	// -- Note that it is possible to set the sensitive detector
	// -- at construction of the G4LogicalVolume (fith argument
	// -- is a G4VSensitiveDetector*).
	// ----------------------------------------------------------
	//Every time the /det/update command is executed this
	//method is called since geometry is recomputed.
	//However we do not need to create a new SD, but reuse the
	//already existing one
	static SensitiveDetector* sensitive = 0;
	if ( !sensitive) {
		sensitive = new SensitiveDetector("/myDet/SiStripSD");
		//We register now the SD with the manager
	  G4SDManager::GetSDMpointer()->AddNewDetector(sensitive);

  }
  logicSensorStrip->SetSensitiveDetector(sensitive);

  if ( isSecondPlaneDUT ) {
	  //With DUT we need to attach to the logical volume of the strips the
	  //sensitive detector, since we only have a pointer to the physical volume
	  //of the second plane we need to do some gymnastic:
	  //1- First we get the logical volume associated to the second plane
	  const G4LogicalVolume* log = physiSecondSensor->GetLogicalVolume();
	  //2- Then we get the first daughter associated to it: the strip
	  // since strip is a replicated volume (G4PVReplica) there is
	  // a single G4PhysicalVolume representing all replicas.
	  // We actually need the LogicalVolume.
	  // Thus we ask the first daughter ::GetDaughter(0) and ask the
	  // associated logical volume
	  log->GetDaughter(0)->GetLogicalVolume()->SetSensitiveDetector(sensitive);
  }

  G4Color red(1.0,0.0,0.0),yellow(1.0,1.0,0.0);
  logicSensorPlane -> SetVisAttributes(new G4VisAttributes(yellow));
  logicSensorStrip -> SetVisAttributes(new G4VisAttributes(yellow));

  return physiSecondSensor;
}

G4VPhysicalVolume* DetectorConstruction::ConstructDeviceUnderTest()
{
  //
  // Device under Test - replaces 2nd Plane of Si Beam Telescope
  //
  G4double halfSensorSizeX = noOfSensorStrips*dutStripPitch/2.;
  G4double halfSensorSizeY = sensorStripLength/2.;
  G4double halfSensorSizeZ = sensorThickness/2.;

  G4Box * solidSensor = new G4Box("SensorDUT",
			   halfSensorSizeX,halfSensorSizeY,halfSensorSizeZ);

  G4LogicalVolume * logicSensorPlane = new G4LogicalVolume(solidSensor, // its solid
      				   silicon,	//its material
      				   "SensorPlaneDUT");	//its name

  G4RotationMatrix * rm = new G4RotationMatrix;
  rm->rotateY(dutTheta);

  physiSecondSensor =
    new G4PVPlacement(rm,
		      posSecondSensor,
		      logicSensorPlane,
		      "DeviceUnderTest",
		      logicWorld,
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
	      halfSensorStripSizeX,halfSensorStripSizeY,halfSensorStripSizeZ);

  G4LogicalVolume * logicSensorStrip = 
    new G4LogicalVolume(solidSensorStrip,silicon,"SensorStripDUT");

  physiSensorStripDUT = 
    new G4PVReplica("SensorStripDUT",		//its name
		    logicSensorStrip,		//its logical volume
		    logicSensorPlane,		//its mother
		    kXAxis,		        //axis of replication
		    noOfSensorStrips,		//number of replica
		    dutStripPitch);
//		    teleStripPitch);	        //witdth of replica

  G4Color red(1.0,0.0,0.0),yellow(1.0,1.0,0.0);
  logicSensorPlane -> SetVisAttributes(new G4VisAttributes(yellow));
  logicSensorStrip -> SetVisAttributes(new G4VisAttributes(red));

  return physiSecondSensor;
}

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
