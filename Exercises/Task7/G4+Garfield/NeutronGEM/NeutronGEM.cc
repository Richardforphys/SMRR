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
/// \file mainNeutronGEM.cc
/// \brief Main program of the NeutronGEM

#include "G4RunManager.hh"
#include "G4ScoringManager.hh"
#include "G4UImanager.hh"
#include "Randomize.hh"

#include "NeutronGEMDetectorConstruction.hh"
#include "NeutronGEMPhysicsList.hh"
#include "NeutronGEMPrimaryGeneratorAction.hh"
#include "NeutronGEMRunAction.hh"
#include "NeutronGEMEventAction.hh"
#include "NeutronGEMStackingAction.hh"
#include "NeutronGEMSteppingAction.hh"
#include "NeutronGEMTrackingAction.hh"
#include "NeutronGEMDataManager.hh"
#include "NeutronGEMHistoManager.hh"

#include "GarfieldG4FastSimulationModel.hh"
#include "GarfieldPhysics.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

#define G4_TRACKING_DEBUG 1
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char** argv) {
	setenv("G4NEUTRONHP_USE_ONLY_PHOTONEVAPORATION", "1", true);
	setenv("G4LIB_BUILD_GDML", "1", true);
	CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);

	//set random seed with system time
	G4long seed = time(NULL);
	CLHEP::HepRandom::setTheSeed(seed);

	// Construct the default run manager
	//
	G4RunManager * runManager = new G4RunManager;
	NeutronGEMDataManager* dataManager = NeutronGEMDataManager::GetInstance();

	if (argc != 1)   // batch mode
			{
		dataManager->setDescription(argv[2]);
		dataManager->setConverterMaterial(argv[3]);
		dataManager->setCathodeMaterial(argv[4]);
		dataManager->setSEEMaterial(argv[5]);
		dataManager->setConverterThickness(atof(argv[6]));
		dataManager->setCathodeThickness(atof(argv[7]));
		dataManager->setSEEThickness(atof(argv[8]));
		dataManager->setDriftThickness(atof(argv[9]));
		dataManager->setDriftField(atof(argv[10]));
	} else {
		dataManager->setDescription("NeutronGEM");
		dataManager->setConverterMaterial("Gd");
		dataManager->setCathodeMaterial("Al");
		dataManager->setSEEMaterial("CsI");
		dataManager->setConverterThickness(100);
		dataManager->setCathodeThickness(0.01);
		dataManager->setSEEThickness(0);
		dataManager->setDriftThickness(3);
		dataManager->setDriftField(0);
	}
	NeutronGEMHistoManager* histoManager = new NeutronGEMHistoManager();
	dataManager->setHistoManager(histoManager);

	GarfieldPhysics* garfieldPhysics = new GarfieldPhysics("GdAndB4C");
	//garfieldPhysics->AddParticleName("alpha", 0.0, 2000.0);
	//garfieldPhysics->AddParticleName("Li7", 0.0, 2000.0);
	//garfieldPhysics->AddParticleName("e-", 0.0, 10000.0);
	//garfieldPhysics->AddParticleName("gamma", 0.0, 10000.0);

	runManager->SetUserInitialization(
			new NeutronGEMPhysicsList(garfieldPhysics));

	NeutronGEMDetectorConstruction* detectorConstruction =
			new NeutronGEMDetectorConstruction();
	runManager->SetUserInitialization(detectorConstruction);

	// Set user action classes
	//
	runManager->SetUserAction(new NeutronGEMPrimaryGeneratorAction());

	NeutronGEMRunAction* runAction = new NeutronGEMRunAction();
	//
	runManager->SetUserAction(runAction);
	//
	runManager->SetUserAction(new NeutronGEMEventAction);
	//
	runManager->SetUserAction(new NeutronGEMStackingAction);

	runManager->SetUserAction(new NeutronGEMSteppingAction);

	runManager->SetUserAction(new NeutronGEMTrackingAction);
	//theApp.Run(kTRUE);
	// Initialize G4 kernel
	//
	runManager->Initialize();

	GarfieldG4FastSimulationModel* garfieldG4FastSimulationModel =
			detectorConstruction->GetGarfieldG4FastSimulationModel();
	garfieldG4FastSimulationModel->SetPhysics(garfieldPhysics);
#ifdef G4VIS_USE

	// Initialise visualisation
	G4VisManager* visManager = new G4VisExecutive;
	// G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
	// G4VisManager* visManager = new G4VisExecutive("Quiet");
	visManager->Initialize();
#endif

	// Get the pointer to the User Interface manager
	G4UImanager* UImanager = G4UImanager::GetUIpointer();

	if (argc != 1)   // batch mode
			{
		G4String fileName = argv[1];
		G4String command = "/control/execute ";
		UImanager->ApplyCommand(command + fileName);
	} else {  // interactive mode : define UI session
#ifdef G4UI_USE
		G4UIExecutive* ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE 
		UImanager->ApplyCommand("/control/execute ./macros/vis.mac");
#endif
		ui->SessionStart();

		delete ui;
#endif
	}

	// Job termination
	// Free the store: user actions, physics_list and detector_description are
	// owned and deleted by the run manager, so they should not be deleted 
	// in the main() program !

#ifdef G4VIS_USE
	delete visManager;

#endif

	delete runManager;
	dataManager->Dispose();
	delete garfieldG4FastSimulationModel;
	return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
