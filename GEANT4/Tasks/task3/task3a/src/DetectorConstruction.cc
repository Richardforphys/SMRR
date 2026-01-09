// $Id: DetectorConstruction.cc 94 2010-01-26 13:18:30Z adotti $
/**
 * @file
 * @brief Implements mandatory user class DetectorConstruction.
 */

#include "DetectorConstruction.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"

#include "G4NistManager.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "Analysis.hh"

#include "G4GeometryManager.hh"
#include "G4SDManager.hh"

DetectorConstruction::DetectorConstruction()
{
  //--------- Material definition ---------
  DefineMaterials();

  //--------- Sizes of the principal geometrical components (solids)  ---------
  ComputeParameters();
}
 
DetectorConstruction::~DetectorConstruction()
{}
 
void DetectorConstruction::DefineMaterials() 
{
  //Get Materials from NIST database
  G4NistManager* man = G4NistManager::Instance();
  man->SetVerbose(1);

  // define NIST materials
  vacuum  = man->FindOrBuildMaterial("G4_Galactic");

  //Tracker
  air     = man->FindOrBuildMaterial("G4_AIR");
  silicon = man->FindOrBuildMaterial("G4_Si");
  //Em calo
  pbw04   = man->FindOrBuildMaterial("G4_PbWO4");
}
 
void DetectorConstruction::ComputeParameters() 
{
  //This function defines the defaults
  //of the geometry construction

  // ** em calo **
  emCaloCentralCrystalWidth = 22*mm;
  emCaloWidth               = 110*mm;
  emCaloLength              = 230*mm;

  // ** Si tracker **
  noOfSensorStrips  = 600;//48;
  sensorStripLength = 10.*mm;
  sensorThickness   = 300.*um;
  teleStripPitch    = 80*um;//20. * um;
  G4double length   = 90*mm;  

  // ** world size and position of detectors are computed **
  gap = 10*mm;
  halfWorldLength = (emCaloLength + length)/2 + gap;
  emCaloZ = length/2; 
  zFirstSensor  = emCaloZ - emCaloLength/2 - length/3;
  zSecondSensor = zFirstSensor - length/3;
  zThirdSensor  = zSecondSensor - length/3;

  G4cout << "### DetectorConstruction: World halfLength(cm)= " 
	 <<  halfWorldLength/cm << G4endl;
}
 
G4VPhysicalVolume* DetectorConstruction::Construct()
{
  //This function is called by G4 when the detector has to be created
  //--------- Definitions of Solids, Logical Volumes, Physical Volumes ---------

  
  //------------------------------
  // World
  //------------------------------
 
  G4double halfWidth = emCaloWidth/2 + gap;
  G4Box * solidWorld= new G4Box("world",halfWidth,halfWidth,halfWorldLength);
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
				 


  //Construction of the three si plane is actually done here
  ConstructTelescope();

  //Construction of the EM calorimeter
  ConstructEMCalo();

  //--------- Visualization attributes -------------------------------

  G4Color
    green(0.0,1.0,0.0),
    blue(0.0,0.0,1.0),
    brown(0.4,0.4,0.1),
    white(1.0,1.0,1.0);

  logicWorld -> SetVisAttributes(new G4VisAttributes(white));
  logicWorld -> SetVisAttributes(G4VisAttributes::Invisible);
    
  //always return the physical World
  //
  return physiWorld;
}

G4VPhysicalVolume* DetectorConstruction::ConstructTelescope()
{
  //
  // 1st Plane of Si tracker
  //
  G4double halfSensorSizeX = noOfSensorStrips*teleStripPitch/2.;
  G4double halfSensorSizeY = sensorStripLength/2.;
  G4double halfSensorSizeZ = sensorThickness/2.;

  G4Box * solidSensor = new G4Box("Sensor",
				  halfSensorSizeX,halfSensorSizeY,halfSensorSizeZ);

  G4LogicalVolume * logicSensorPlane = new G4LogicalVolume(solidSensor, // its solid
							   silicon,	//its material
							   "SensorPlane"); //its name

  physiFirstSensor = new G4PVPlacement(0,	//no rotation
				       G4ThreeVector(0,0,zFirstSensor),
				       logicSensorPlane,	 //its logical volume
				       "FirstSensor",		//its name
				       logicWorld,		//its mother  volume
				       false,			//no boolean operation
				       0);			//copy number


  // 2nd Plane of Si tracker

  physiSecondSensor = new G4PVPlacement(0,//rm,
					G4ThreeVector(0,0,zSecondSensor),
					logicSensorPlane,
					"SecondSensor",
					logicWorld,
					false,
					1);			//copy number

  // 3rd Plane of Si tracker
  physiThirdSensor = new G4PVPlacement(0,
				       G4ThreeVector(0,0,zThirdSensor),
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

  G4Color red(1.0,0.0,0.0),yellow(1.0,1.0,0.0);
  logicSensorPlane -> SetVisAttributes(new G4VisAttributes(red));
  logicSensorStrip -> SetVisAttributes(new G4VisAttributes(yellow));
  
  return physiSecondSensor;
}


G4VPhysicalVolume* DetectorConstruction::ConstructEMCalo()
{
  G4double halfEmCaloSizeX = emCaloWidth/2;
  G4double halfEmCaloSizeY = emCaloWidth/2;
  G4double halfEmCaloSizeZ = emCaloLength/2;
  G4double halfEmCentralSizeX = emCaloCentralCrystalWidth/2;
  G4double halfEmCentralSizeY = emCaloCentralCrystalWidth/2;

  G4Box* emSolid = new G4Box("emCaloSolid",halfEmCaloSizeX,halfEmCaloSizeY,halfEmCaloSizeZ);

  G4LogicalVolume* emLogic = new G4LogicalVolume( emSolid,//its solid
						  pbw04, //its material
						  "emCaloLogic");//its name
  emCalo = new G4PVPlacement(0, //no rotation
			     G4ThreeVector(0,0,emCaloZ),//translation
			     emLogic, //its logical volume
			     "EmCalo",//its name
			     logicWorld,//its mother volume
			     false, //no boolean operation
			     10); //copy number

  Analysis::GetInstance()->SetEcalZposition(emCaloZ - halfEmCaloSizeZ);


  G4Box* emCentralSolid=new G4Box("emCentralSolid",halfEmCentralSizeX,halfEmCentralSizeY,halfEmCaloSizeZ);
  G4LogicalVolume* emCentralLogic = new G4LogicalVolume( emCentralSolid,pbw04,"emCentralLogic");
  emCaloCentralCrystal = new G4PVPlacement(0, 
					   G4ThreeVector(0,0,0), 
					   emCentralLogic , 
					   "EmCentral", 
					   emLogic ,
					   false, 
					   11);

  G4Color yellow(1,1,0);
  emLogic->SetVisAttributes(new G4VisAttributes(yellow));
  G4Color red(1,0,0);
  emCentralLogic->SetVisAttributes(new G4VisAttributes(red));
  return emCalo;
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
