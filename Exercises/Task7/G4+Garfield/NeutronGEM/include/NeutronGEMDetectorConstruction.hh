//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id$
//
/// \file NeutronGEMDetectorConstruction.hh
/// \brief Definition of the NeutronGEMDetectorConstruction class

#ifndef NeutronGEMDetectorConstruction_h
#define NeutronGEMDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class NeutronGEMDataManager;
class GarfieldG4FastSimulationModel;
class G4Material;
class G4Box;


/// Detector construction class to define materials and geometry.
///
/// Crystals are positioned in Ring, with an appropriate rotation matrix. 
/// Several copies of Ring are placed in the full detector.

class NeutronGEMDetectorConstruction: public G4VUserDetectorConstruction {

public:
	NeutronGEMDetectorConstruction();
	virtual ~NeutronGEMDetectorConstruction();
	GarfieldG4FastSimulationModel* GetGarfieldG4FastSimulationModel();
public:
	virtual G4VPhysicalVolume* Construct();
private:
	G4bool fCheckOverlaps;
	NeutronGEMDataManager* fDataManager;
	GarfieldG4FastSimulationModel* fGarfieldG4FastSimulationModel;

	void DefineMaterials();
	void CreateScorers();

	G4double fWorldSize;
	G4double fDetectorSize;
	G4double fCathodeThickness;
	G4double fConverterThickness;
	G4double fSEEThickness;
	G4double fDriftThickness;
	G4double fDriftField;

	G4Material* fWorldMaterial;
	G4Material* fCathodeMaterial;
	G4Material* fConverterMaterial;
	G4Material* fSEEMaterial;
	G4Material* fDriftMaterial;

	G4Box* fSolidWorld;
	G4LogicalVolume* fLogicalWorld;
	G4VPhysicalVolume* fPhysicalWorld;
	G4Box* fSolidConverter;
	G4LogicalVolume* fLogicalConverter;
	G4VPhysicalVolume* fPhysicalConverter;
	G4Box* fSolidCathode;
	G4LogicalVolume* fLogicalCathode;
	G4VPhysicalVolume* fPhysicalCathode;
	G4Box* fSolidDrift;
	G4LogicalVolume* fLogicalDrift;
	G4VPhysicalVolume* fPhysicalDrift;
	G4Box* fSolidSEE;
	G4LogicalVolume* fLogicalSEE;
	G4VPhysicalVolume* fPhysicalSEE;


};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

