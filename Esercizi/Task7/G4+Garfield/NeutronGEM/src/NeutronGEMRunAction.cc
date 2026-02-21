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
/// \file NeutronGEMRunAction.cc
/// \brief Implementation of the NeutronGEMRunAction class

#include "NeutronGEMRunAction.hh"
#include "NeutronGEMPrimaryGeneratorAction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "NeutronGEMDataManager.hh"
#include "NeutronGEMHistoManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NeutronGEMRunAction::NeutronGEMRunAction()
	: G4UserRunAction(), fNumberOfEvents(0)
{  

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NeutronGEMRunAction::~NeutronGEMRunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NeutronGEMRunAction::BeginOfRunAction(const G4Run* run)
{ 
	fTime = time(NULL);

	G4cout << "### Run " << run->GetRunID() << " started" << G4endl;
	NeutronGEMDataManager* dataManager =
			NeutronGEMDataManager::GetInstance();
	dataManager->setNumberOfEvents(run->GetNumberOfEventToBeProcessed());
	(dataManager->getHistoManager())->book();

	//inform the runManager to save random number seed
	G4RunManager::GetRunManager()->SetRandomNumberStore(false);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NeutronGEMRunAction::EndOfRunAction(const G4Run* run)
{
	fTime = time(NULL) - fTime;

	fNumberOfEvents = run->GetNumberOfEvent();
	if (fNumberOfEvents == 0) return;
	G4cout << "### Run " << run->GetRunID() << " of " << fNumberOfEvents << " took " << fTime << " s" << G4endl;
	NeutronGEMDataManager* dataManager =
				NeutronGEMDataManager::GetInstance();
	dataManager->setNumberOfEvents(fNumberOfEvents);
	(dataManager->getHistoManager())->save();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
