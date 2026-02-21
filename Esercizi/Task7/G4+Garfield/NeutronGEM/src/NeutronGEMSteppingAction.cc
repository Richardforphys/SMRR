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
/// \file field/field04/src/NeutronGEMSteppingAction.cc
/// \brief Implementation of the NeutronGEMSteppingAction class
//
//
#include "G4Track.hh"
#include "G4SteppingManager.hh"
#include "G4ParticleTypes.hh"
#include "G4SystemOfUnits.hh"
#include "G4StepStatus.hh"
#include "G4VSolid.hh"
#include "G4VPhysicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4GeometryTolerance.hh"
#include "G4Box.hh"
#include "NeutronGEMSteppingAction.hh"
#include "NeutronGEMDataManager.hh"
#include "NeutronGEMHistoManager.hh"
#include "NeutronGEMTrackInformation.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NeutronGEMSteppingAction::NeutronGEMSteppingAction() :
		G4UserSteppingAction() {

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NeutronGEMSteppingAction::~NeutronGEMSteppingAction() {
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NeutronGEMSteppingAction::UserSteppingAction(const G4Step* theStep) {
	NeutronGEMDataManager* dataManager = NeutronGEMDataManager::GetInstance();
	NeutronGEMHistoManager* histoManager = dataManager->getHistoManager();
	NeutronGEMTrackInformation* trackInfo = 0;
	G4String ancestorName = "neutron";
	if (theStep->GetTrack()->GetUserInformation()) {
		trackInfo =
				(NeutronGEMTrackInformation*) (theStep->GetTrack()->GetUserInformation());
		ancestorName = trackInfo->GetParticleDefinition()->GetParticleName();
	}

	G4StepPoint* preStep = theStep->GetPreStepPoint();
	G4StepPoint* postStep = theStep->GetPostStepPoint();
	G4TouchableHandle theTouchable = preStep->GetTouchableHandle();

	G4ThreeVector worldPosition = preStep->GetPosition();
	G4ThreeVector localPosition =
			theTouchable->GetHistory()->GetTopTransform().TransformPoint(
					worldPosition);

	G4VPhysicalVolume* physVol = theTouchable->GetVolume();

	G4Track* fTrack = theStep->GetTrack();
	G4double energy = theStep->GetPreStepPoint()->GetKineticEnergy()/ CLHEP::MeV;
	G4double energy_eV = theStep->GetPreStepPoint()->GetKineticEnergy()/ CLHEP::eV;
	G4ParticleDefinition* thePartDef = fTrack->GetDefinition();
	G4String partName = thePartDef->GetParticleName();
	G4String partType = thePartDef->GetParticleType();
	G4String volumeNamePresent = physVol->GetName();
	double x = localPosition.getX() / CLHEP::cm;
	double y = localPosition.getY() / CLHEP::cm;
	double z = localPosition.getZ() / CLHEP::cm;

	double dx = fTrack->GetMomentumDirection().getX();
	double dy = fTrack->GetMomentumDirection().getY();
	double dz = fTrack->GetMomentumDirection().getZ();
	G4String volumeNameNext = "";
	if (fTrack->GetNextVolume()) {
		volumeNameNext = fTrack->GetNextVolume()->GetName();
	}

	if (fTrack->GetCurrentStepNumber() == 1) {
		if (volumeNamePresent == "Converter") {
			if (partName == "gamma") {
				dataManager->increaseCounter(4);
				histoManager->fillHistogram(4, energy);

			} else if (partName == "e-") {
				if (fTrack->GetParentID() == 1) {
					dataManager->increaseCounter(1);
					histoManager->fillHistogram(1, energy);

				} else {
					dataManager->increaseCounter(5);
					histoManager->fillHistogram(5, energy );
				}
			} else if (partName == "alpha") {
				dataManager->increaseCounter(2);
				histoManager->fillHistogram(2, energy);
			} else if (partName == "Li7") {
				dataManager->increaseCounter(3);
				histoManager->fillHistogram(3, energy);
			}
			else if (fTrack->GetCreatorProcess()->GetProcessName() == "nCapture")
			{
				dataManager->increaseCounter(0);
			}


		} else if (volumeNamePresent == "SEE") {
			//
		} else if (volumeNamePresent == "Drift") {
			if (partName == "e-") {
				if (ancestorName == "e-") {
					dataManager->increaseCounter(11);
					histoManager->fillHistogram(11, energy);
					histoManager->Fill3DEnergyElectrons(1, x, y, z,
							energy_eV );
					histoManager->AddClustersConversionElectrons();
				} else if (ancestorName == "gamma") {
					dataManager->increaseCounter(14);
					histoManager->fillHistogram(14, energy );
					histoManager->Fill3DEnergyElectrons(5, x, y, z,
							energy_eV);
					histoManager->AddClustersOtherElectrons();
				}
			} else if (ancestorName == "alpha") {
				dataManager->increaseCounter(12);
				histoManager->fillHistogram(12, energy);
				histoManager->Fill3DEnergyElectrons(2, x, y, z,
						energy_eV);
				histoManager->AddClustersAlphas();

			} else if (ancestorName == "Li7") {
				dataManager->increaseCounter(13);
				histoManager->fillHistogram(13, energy);
				histoManager->Fill3DEnergyElectrons(3, x, y, z,
						energy_eV);
				histoManager->AddClustersLiIons();

			}
		}
	}

	if (volumeNamePresent == "Drift") {
		G4double edep = theStep->GetTotalEnergyDeposit()/MeV;
		if (partName == "e-") {
			if (ancestorName == "e-") {
				histoManager->AddEnergyConversionElectrons(edep);

			} else {
				histoManager->AddEnergyOtherElectrons(edep);
			}
		} else if (partName == "gamma") {
			histoManager->AddEnergyGammas(edep);
		} else if (partName == "alpha") {
			histoManager->AddEnergyAlphas(edep);
		} else if (partName == "Li7") {
			histoManager->AddEnergyLiIons(edep);
		}
	}

	if (postStep->GetStepStatus() == fGeomBoundary) {
		if (volumeNameNext == "SEE") {
			if (volumeNamePresent == "Cathode") {
				if (partName == "e-") {
					//dataManager->increaseCounter(2);
					//histoManager->fillHistogram(2, energy / CLHEP::MeV);
				}
			}
		} else if (volumeNameNext == "Drift") {
			if (volumeNamePresent == "SEE" || volumeNamePresent == "Cathode") {
				double d = sqrt(x * x + y * y);
				if (partName == "gamma") {
					dataManager->increaseCounter(9);
					histoManager->fillHistogram(9, energy);
					histoManager->fillHistogram(19, d );
					histoManager->Fill2DPositionArrivalDrift(4, x, y);

				} else if (partName == "alpha") {
					dataManager->increaseCounter(7);
					histoManager->fillHistogram(7, energy);
					histoManager->fillHistogram(17, d );
					histoManager->Fill2DPositionArrivalDrift(2, x, y);

				} else if (partName == "Li7") {
					dataManager->increaseCounter(8);
					histoManager->fillHistogram(8, energy);
					histoManager->fillHistogram(18, d );
					histoManager->Fill2DPositionArrivalDrift(3, x, y);

				} else if (partName == "e-") {

					if (ancestorName == "e-") {
						dataManager->increaseCounter(6);
						histoManager->fillHistogram(6, energy );
						histoManager->fillHistogram(16, d );
						histoManager->Fill2DPositionArrivalDrift(1, x, y);
					}
					else
					{
						dataManager->increaseCounter(10);
						histoManager->fillHistogram(10, energy);
						histoManager->fillHistogram(20, d );
						histoManager->Fill2DPositionArrivalDrift(5, x, y);
					}
				}

			}

		}

	}
}

