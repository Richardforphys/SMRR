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
/// \file NeutronGEMEventAction.cc
/// \brief Implementation of the NeutronGEMEventAction class

#include "NeutronGEMEventAction.hh"
#include "NeutronGEMDataManager.hh"
#include "NeutronGEMHistoManager.hh"
#include "G4UImanager.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4THitsMap.hh"
#include "G4UnitsTable.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NeutronGEMEventAction::NeutronGEMEventAction() :
		G4UserEventAction(), fPrintModulo(10000) {
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NeutronGEMEventAction::~NeutronGEMEventAction() {
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NeutronGEMEventAction::BeginOfEventAction(const G4Event* evt) {
	G4int evtNb = evt->GetEventID();
	NeutronGEMDataManager* dataManager = NeutronGEMDataManager::GetInstance();
	dataManager->resetCounters();
	dataManager->getHistoManager()->BeginOfEvent();
	if (evtNb % fPrintModulo == 0) {
		time_t rawtime;
		struct tm * timeinfo;
		time(&rawtime);
		timeinfo = localtime(&rawtime);
		char theTime[50];
		strftime(theTime, 50, "%Y-%m-%d-%H-%M-%S", timeinfo);
		G4cout << "\n---> Begin of event: " << evtNb << " " << theTime << G4endl;
	}

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NeutronGEMEventAction::EndOfEventAction(const G4Event* evt) {
	NeutronGEMDataManager* dataManager = NeutronGEMDataManager::GetInstance();
	dataManager->getHistoManager()->EndOfEvent();
	G4int* counters = dataManager->getCounters();

	if (counters[1] >= 1) {
		counters[16] = 1;
	}

	if (counters[2] >= 1) {
		counters[17] = 1;
	}
	if (counters[3] >= 1) {
		counters[18] = 1;
	}
	if (counters[4] >= 1) {
		counters[19] = 1;
	}
	if (counters[5] >= 1) {
		counters[20] = 1;
	}
	if (counters[6] >= 1) {
		counters[21] = 1;
	}

	if (counters[7] >= 1) {
		counters[22] = 1;
	}
	if (counters[8] >= 1) {
		counters[23] = 1;
	}

	if (counters[9] >= 1) {
		counters[24] = 1;
	}
	if (counters[10] >= 1) {
		counters[25] = 1;
	}
	if (counters[11] >= 1) {
		counters[26] = 1;
	}

	if (counters[12] >= 1) {
		counters[27] = 1;
	}
	if (counters[13] >= 1) {
		counters[28] = 1;
	}

	if (counters[14] >= 1) {
		counters[29] = 1;
	}
	if (counters[15] >= 1) {
		counters[30] = 1;
	}

	(dataManager->getHistoManager())->fillTree(counters);
	//G4int evtNb = evt->GetEventID();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
