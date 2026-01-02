# Task 1

### Detector: Hadronic Calorimeter

In generale fate attenzione ai file con #include
1. Modificare DetectorConstruction.cc  → Cercare la parte con EDITME
2. Nella physics list che particelle sono abilitate? Abilitare muoni e pioni
3. Nella PrimaryGenerationAction  → Comprendere come è fatta la sorgente primaria
4. Guardare file vis.mac

## Solution
1. 
```c++
//Before
G4Tubs* hadLayerSolid = new G4Tubs( "HadCaloLayerSolid", //its name
                                    EDITME:innerradius ,
                                    EDITME:outerradius ,
                                    EDITME:length,
                                    EDITME:start_angle,
                                    EDITME:end_angle);

//After
G4Tubs* hadLayerSolid = new G4Tubs( "HadCaloLayerSolid", //its name
	                                    0,
	                                    hadCaloRadius,
	                                    hadCaloLArThickness/2,
	                                    0,
	                                    CLHEP::twopi);
```
2. 
```c++
//Before
G4LogicalVolume* hadLayerLogic = new G4LogicalVolume(EDITME:solid_pointer,
														 EDITME:material,
														 "HadLayerLogic");//its name

//After
G4LogicalVolume* hadLayerLogic = new G4LogicalVolume(hadLayerSolid,
														 lar,
														 "HadLayerLogic");//its name
```

3. 
```c++
//Before
new G4PVPlacement(EDITME:rotation,
						  position, //position, do not change this, already done for you
						  EDITME:logical_volume,
						  "HadCaloLayer",//a name
						  EDITME:mother_logical_volume,
						  false, //leave this like this: not used
						  ++layerCopyNum);//The unique number, it will be 1001+layerIndex

//After
new G4PVPlacement(0,
						  position, //position, do not change this, already done for you
						  hadLayerLogic,
						  "HadCaloLayer",//a name
						  hadCaloLogic,
						  false, //leave this like this: not used
						  ++layerCopyNum);//The unique number, it will be 1001+layerIndex
``` 

4. Ab initio sono abilitate solo:
    1. Geantino (pseudo-particella)
    2. GEantino carico (pseudo-particella)
```c++
//Before
void PhysicsList::ConstructParticle()
{
  // In this method, static member functions should be called
  // for all particles which you want to use.
  // This ensures that objects of these particle types will be
  // created in the program. 

  // pseudo-particles
  G4Geantino::GeantinoDefinition();
  G4ChargedGeantino::ChargedGeantinoDefinition();

  // define gamma, e+, e- and some charged Hadrons
  emPhysicsList->ConstructParticle();

  /*
// gamma
  G4Gamma::Gamma();

// leptons
  G4Electron::Electron();
  G4Positron::Positron();
  G4MuonPlus::MuonPlus();
  G4MuonMinus::MuonMinus();
  */

  // mesons
  //  G4PionPlus::PionPlusDefinition();
  //  G4PionMinus::PionMinusDefinition();

}

//After
void PhysicsList::ConstructParticle()
{
  // In this method, static member functions should be called
  // for all particles which you want to use.
  // This ensures that objects of these particle types will be
  // created in the program. 

  // pseudo-particles
  G4Geantino::GeantinoDefinition();
  G4ChargedGeantino::ChargedGeantinoDefinition();

  // define gamma, e+, e- and some charged Hadrons
  emPhysicsList->ConstructParticle();

  
  // gamma
  //G4Gamma::Gamma();

  // leptons
  //G4Electron::Electron();
  //G4Positron::Positron();
  G4MuonPlus::MuonPlus();
  G4MuonMinus::MuonMinus();
  

  // mesons
  G4PionPlus::PionPlusDefinition();
  G4PionMinus::PionMinusDefinition();

}

``` 
### To run
```bash
# while in task1b dir
$ mkdir task1b_build
$ cd task1b_build
$ source /home/ricca/SMRR/GEANT4/geant4_install/bin/geant4.sh
$ cmake -DCMAKE_PREFIX_PATH=/home/ricca/SMRR/GEANT4/geant4_install  /home/ricca/SMRR/GEANT4/Tasks/task1b
$ make -j4 VERBOSE=1
```

Verranno fuori errori...per correggere
```c++
//File DetectorConstruction.cc
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

//...sostituire logicWorld -> SetVisAttributes(G4VisAttributes::Invisible); con
logicWorld->SetVisAttributes(G4VisAttributes::GetInvisible());

//File PrimaryGeneratorAction.cc-->mancano includes
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "G4SPSEneDistribution.hh"
#include "G4SPSPosDistribution.hh"
#include "G4SPSAngDistribution.hh"

//File PhysicsList.cc-->mancano includes
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
```