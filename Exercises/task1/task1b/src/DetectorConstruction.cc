// $Id: DetectorConstruction.cc 94 2010-01-26 13:18:30Z adotti $
/**
 * @file
 * @brief Implements mandatory user class DetectorConstruction.
 */

#include "DetectorConstruction.hh"
//#include "DetectorMessenger.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"

#include "G4GeometryTolerance.hh"
#include "G4GeometryManager.hh"
#include "G4NistManager.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SystemOfUnits.hh"
//#include "SensitiveDetector.hh"
#include "G4SDManager.hh"

DetectorConstruction::DetectorConstruction()
{
	//--------- Material definition ---------
	DefineMaterials();

	//--------- Sizes of the principal geometrical components (solids)  ---------
	ComputeParameters();
}
 
DetectorConstruction::~DetectorConstruction()
{
}
 
void DetectorConstruction::DefineMaterials() 
{
	//Get Materials from NIST database
	G4NistManager* man = G4NistManager::Instance();
	man->SetVerbose(0);

	// define NIST materials
	vacuum  = man->FindOrBuildMaterial("G4_Galactic");

	//Tracker
	air     = man->FindOrBuildMaterial("G4_AIR");
	silicon = man->FindOrBuildMaterial("G4_Si");
	//Em calo
	pbw04   = man->FindOrBuildMaterial("G4_PbWO4");
	//Had calo

	lar = man->FindOrBuildMaterial("G4_lAr");
	fe = man->FindOrBuildMaterial("G4_Fe");

}
 
void DetectorConstruction::ComputeParameters() 
{
	//This function defines the defaults
	//of the geometry construction

	// ** world **
	halfWorldLength = 5* m;

	// ** em calo **
	emCaloCentralCrystalWidth = 22*mm;
	emCaloWidth               = 110*mm;
	emCaloLength              = 230*mm;
	posEmCalo = -G4ThreeVector(0,0,emCaloLength/2);

	// ** Si tracker **
	noOfSensorStrips = 600;//48;
	sensorStripLength = 10.*mm;
	sensorThickness = 300.*um;
	teleStripPitch  = 80*um;//20. * um;
	posFirstSensor  = G4ThreeVector(0., 0., -190*mm - emCaloLength);
	posSecondSensor = G4ThreeVector(0., 0., -100*mm - emCaloLength);
	posThirdSensor  = G4ThreeVector(0., 0., -10.*mm - emCaloLength);

	// ** Had Calorimeter **
	hadCaloLArThickness = 8*mm;
	hadCaloFeThickness = 20*mm;
	hadCaloRadius = 800*mm;
	hadCaloNumLayers = 80;
	posHadCalo = G4ThreeVector(0,0,(hadCaloLArThickness+hadCaloFeThickness)*hadCaloNumLayers/2);
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
				 


	//Construction of the three si plane is actually done here
	ConstructTelescope();

	//Construction of the EM calorimeter
	ConstructEMCalo();

	//Construction of the Had calorimeter
	//This method is called to construct the HAD calorimeter
	ConstructHadCalo();

	//--------- Visualization attributes -------------------------------

	G4Color
		green(0.0,1.0,0.0),
		blue(0.0,0.0,1.0),
		brown(0.4,0.4,0.1),
		white(1.0,1.0,1.0);

	logicWorld -> SetVisAttributes(new G4VisAttributes(white));
	logicWorld -> SetVisAttributes(new G4VisAttributes(false));
    
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
			"SensorPlane");	//its name

	physiFirstSensor = new G4PVPlacement(0,	//no rotation
				  posFirstSensor,
				  logicSensorPlane,		//its logical volume
				  "FirstSensor",		//its name
				  logicWorld,		//its mother  volume
				  false,			//no boolean operation
				  0);			//copy number


	// 2nd Plane of Si tracker

	physiSecondSensor = new G4PVPlacement(0,//rm,
						  posSecondSensor,
						  logicSensorPlane,
						  "SecondSensor",
						  logicWorld,
						  false,
						  1);			//copy number

	// 3rd Plane of Si tracker
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

  G4Color red(1.0,0.0,0.0),yellow(1.0,1.0,0.0);
  logicSensorPlane -> SetVisAttributes(new G4VisAttributes(yellow));
  logicSensorStrip -> SetVisAttributes(new G4VisAttributes(red));

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
							   posEmCalo,//translation
							   emLogic, //its logical volume
							   "EmCalo",//its name
							   logicWorld,//its mother volume
							   false, //no boolean operation
							   10); //copy number

	G4Box* emCentralSolid=new G4Box("emCentralSolid",halfEmCentralSizeX,halfEmCentralSizeY,halfEmCaloSizeZ);
	G4LogicalVolume* emCentralLogic = new G4LogicalVolume( emCentralSolid,pbw04,"emCentralLogic");
	emCaloCentralCrystal = new G4PVPlacement( 0 , G4ThreeVector(0,0,0), emCentralLogic , "EmCentral", emLogic ,false, 11);

	G4Color yellow(1,1,0);
	emLogic->SetVisAttributes(new G4VisAttributes(yellow));
	G4Color red(1,0,0);
	emCentralLogic->SetVisAttributes(new G4VisAttributes(red));
	return emCalo;
}


G4VPhysicalVolume* DetectorConstruction::ConstructHadCalo()
{
	//Dimension of the HAD calo along Z (beam line)
	G4double halfHadCaloHalfZ = (hadCaloFeThickness+hadCaloLArThickness)*hadCaloNumLayers/2;

	//Create the HAD calo main volume: a cylinder
	G4Tubs* hadCaloSolid = new G4Tubs( "hadCaloSolid",//its name
										0, //inner radius
										hadCaloRadius,//outer radius
										halfHadCaloHalfZ, //half length in z: number of layers
										0, //Starting angle in phi
										CLHEP::twopi);//Ending angle in phi, twopi = 2*3.1415...

	//Create the HAD calo main volume logical volume.
	//A logical volume is made of: a solid and a material
	//A name must also be given
	G4LogicalVolume* hadCaloLogic = new G4LogicalVolume( hadCaloSolid,//its solid
														 fe,//its material
														 "HadCaloLogic");//its name

	//This is the HAD calo copy number that uniquely identifies the volume
	G4int hadCaloCopyNum = 1000;
	//We finish now the creation of the HAD calorimeter creating a placement:
	//We place the volume in the "World"
	hadCalo = new G4PVPlacement( 0, //no rotation
							     posHadCalo, //translation
							     hadCaloLogic,//its logical volume
							     "HadCalo",//its name
							     logicWorld,//its mother volume
							     false,
							     hadCaloCopyNum);//copy number
	//-------------------------
	//Exercise 1 of task1b
	//-------------------------
	//We want now to create a layer of active material: LAr
	//We need now to create the solid: a cylinder.
	//The dimensions are the same as the hadCaloSolid except the
	//half dimension in Z: it is now hadCaloLArThickness/2
	//We now make layers of LAr and add them to the hadronic calo logic
	G4Tubs* hadLayerSolid = new G4Tubs( "HadCaloLayerSolid", //its name
	                                    0,
										hadCaloRadius,
	                                    hadCaloLArThickness/2,
	                                    0*deg,
	                                    CLHEP::twopi);


	//-------------------------
	//Exercise 2 of task1b
	//-------------------------
	//We need now to create the logical volume.
	//A logical volume is build with a pointer to a solid
	//a pointer to a material and a name
	G4LogicalVolume* hadLayerLogic = new G4LogicalVolume(hadLayerSolid,
														 lar,//its material
														 "HadLayerLogic");//its name

	//-------------------------
	//Exercise 3 of task1b
	//-------------------------
	//We need now to place in the HAD calo volume many "copies" of the liquid argon layer.
	//We do it with a for-loop making several placements.
	//Remember: when you create a placement you need to specify the positions in the mother volume
	//coordinate frame. In this case the mother wolume is the hadCaloLogic logical volume
	//The postion is parametrized depending on the layer number, we create some variables to help these
	//placements
	//Translation of one Layer with respect previous Layer
	G4ThreeVector absorberLayer(0,0,hadCaloFeThickness);
	G4ThreeVector activeLayer(0,0,hadCaloLArThickness);
	G4int layerCopyNum = hadCaloCopyNum;
	for ( int layerIdx = 0 ; layerIdx < hadCaloNumLayers ; ++layerIdx )
	{
		G4ThreeVector position = (layerIdx+1)*absorberLayer + (layerIdx+0.5)*activeLayer;
		position -= G4ThreeVector(0,0,halfHadCaloHalfZ);//Position is w.r.t. center of mother volume: the hadCaloLogic
		new G4PVPlacement(0, //no rotation
						  position, //position, do not change this, already done for you
						  hadLayerLogic,
						  "HadCaloLayer",//a name
						  hadCaloLogic,
						  false, //leave this like this: not used
						  ++layerCopyNum);//The unique number, it will be 1001+layerIndex
	}

	G4Colour green(0,1,0);
	G4Colour white(1,1,1);
	G4Colour brown(0.4,0.4,0.1);
	hadCaloLogic->SetVisAttributes(new G4VisAttributes(brown));
	hadLayerLogic->SetVisAttributes(new G4VisAttributes(brown));
	return hadCalo;
}
