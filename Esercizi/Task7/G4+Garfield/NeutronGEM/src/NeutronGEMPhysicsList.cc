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
/// \file NeutronGEMPhysicsList.cc
/// \brief Implementation of the NeutronGEMPhysicsList class

#include "GarfieldPhysics.hh"

#include "NeutronGEMPhysicsList.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmProcessOptions.hh"
#include "G4UnitsTable.hh"
#include "G4FastSimulationManagerProcess.hh"
#include "G4PAIModel.hh"
#include "G4PAIPhotModel.hh"
#include "G4eIonisation.hh"
#include "G4RegionStore.hh"
#include "G4eMultipleScattering.hh"
#include "G4eBremsstrahlung.hh"
#include "G4LossTableManager.hh"
#include "G4EmConfigurator.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NeutronGEMPhysicsList::NeutronGEMPhysicsList(GarfieldPhysics *garfieldPhysics) :
		G4VModularPhysicsList(), fGarfieldPhysics(garfieldPhysics) {
	G4int verb = 1;
	SetVerboseLevel(verb);
	defaultCutValue = 1 * CLHEP::mm;
	RegisterPhysics(new G4EmLivermorePhysics(verb));
	RegisterPhysics(new G4HadronPhysicsQGSP_BIC_HP(verb));

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NeutronGEMPhysicsList::~NeutronGEMPhysicsList() {

}

void NeutronGEMPhysicsList::AddParameterisation() {
	//G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();
	// -- Fast simulation manager process for "mass geometry":
	G4FastSimulationManagerProcess* fastSimProcess_garfield =
			new G4FastSimulationManagerProcess("G4FSMP_garfield");

	theParticleIterator->reset();
	while ((*theParticleIterator)()) {

		G4ParticleDefinition* particle = theParticleIterator->value();
		G4ProcessManager* pmanager = particle->GetProcessManager();

		if (fGarfieldPhysics->FindParticleName(particle->GetParticleName())) {
			pmanager->AddDiscreteProcess(fastSimProcess_garfield);
		}
/*
		if (particle->GetParticleName() == "e-"
				|| particle->GetParticleName() == "e+") {
			G4EmConfigurator* config =
					G4LossTableManager::Instance()->EmConfigurator();
			G4LossTableManager::Instance()->SetVerbose(1);

			G4PAIModel* pai = new G4PAIModel(particle, "PAIModel");
			config->SetExtraEmModel(particle->GetParticleName(), "eIoni", pai,
					"RegionGarfield", 0.0, 100. * TeV, pai);


			G4PAIPhotModel* paiPhot = new G4PAIPhotModel(particle,
					"G4PAIPhotModel");
			config->SetExtraEmModel(particle->GetParticleName(), "eIoni",
					paiPhot, "RegionGarfield", 0.0, 100. * TeV, paiPhot);


		}
		else if (particle->GetParticleName() == "alpha"
				|| particle->GetParticleName() == "Li7") {
			G4EmConfigurator* config =
					G4LossTableManager::Instance()->EmConfigurator();
			G4LossTableManager::Instance()->SetVerbose(1);

			G4PAIModel* pai = new G4PAIModel(particle, "PAIModel");
			config->SetExtraEmModel(particle->GetParticleName(), "hIoni", pai,
					"RegionGarfield", 0.0, 100. * TeV, pai);
		}
*/
	}
}

void NeutronGEMPhysicsList::SetCuts() {
	G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(1.*keV,100. * MeV);
	if (verboseLevel > 0) {
		G4cout << "PhysicsList::SetCuts:";
		G4cout << "CutLength : " << G4BestUnit(defaultCutValue, "Length")
				<< G4endl;
	}
	G4VModularPhysicsList::SetCutsWithDefault();
	//if (verboseLevel >0) DumpCutValuesTable();
}

void NeutronGEMPhysicsList::ConstructParticle() {
	G4VModularPhysicsList::ConstructParticle();
}

void NeutronGEMPhysicsList::ConstructProcess() {
	G4VModularPhysicsList::ConstructProcess();
	AddParameterisation();
	G4EmProcessOptions emOptions;
	emOptions.SetFluo(true);
	emOptions.SetAuger(true);
	emOptions.SetPIXE(true);
}

