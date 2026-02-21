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
/// \file NeutronGEMDataManager.hh
/// \brief Definition of the NeutronGEMDataManager class

#ifndef NeutronGEMDataManager_h
#define NeutronGEMDataManager_h 1

#define NUM_COUNTERS 31
#include "G4ThreeVector.hh"
#include <map>
#include <vector>

class NeutronGEMHistoManager;

class NeutronGEMDataManager {
public:
	// With description

	static NeutronGEMDataManager* GetInstance();
	static void Dispose();
	NeutronGEMHistoManager* getHistoManager();

	void setHistoManager(NeutronGEMHistoManager* histoManager);
	void setDescription(G4String description);
	void setFilenameGDML(G4String filenameGDML);
	G4String getFilenameGDML();
	G4String getDescription();
	G4int getNumberOfEvents();
	void setNumberOfEvents(G4int numberOfEvents);

	void setCathodeThickness(G4double thickness);
	G4double getCathodeThickness();
	void setConverterThickness(G4double thickness);
	G4double getConverterThickness();
	void setSEEThickness(G4double thickness);
	G4double getSEEThickness();
	void setDriftThickness(G4double thickness);
	G4double getDriftThickness();

	void setCathodeMaterial(G4String material);
	void setConverterMaterial(G4String material);
	void setSEEMaterial(G4String material);
	G4String getCathodeMaterial();
	G4String getConverterMaterial();
	G4String getSEEMaterial();

	G4double getDriftField();
	void setDriftField(G4double driftField);

	void resetCounters();
	G4int* getCounters();
	void increaseCounter(int number);

private:
	NeutronGEMDataManager();
	~NeutronGEMDataManager();

	G4String fDescription;
	G4String fFilenameGDML;
	G4double fNumberOfEvents;

	NeutronGEMHistoManager* fHistoManager;
	static NeutronGEMDataManager* fDataManager;
	G4int* fCounters;


	G4double fCathodeThickness;
	G4double fConverterThickness;
	G4double fSEEThickness;
	G4double fDriftThickness;

	G4double fDriftField;
	G4String fCathodeMaterial;
	G4String fConverterMaterial;
	G4String fSEEMaterial;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

