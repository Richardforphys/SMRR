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
// $Id: HistoManager.hh 48195 2011-02-02 15:33:39Z jjacquem $
// GEANT4 tag $Name: geant4-09-04 $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef HistoManager_h
#define HistoManager_h 1

#define NUM_HISTOGRAMS 30
#define NUM_HISTOGRAMS_2D 5
#define NUM_HISTOGRAMS_3D 5

#include "globals.hh"
#include "G4DataVector.hh"
#include <vector>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class TFile;
class TTree;
class TH1D;
class TH2D;
class TH3D;
class TBranch;

class G4Step;
class G4ElectronIonPair;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class NeutronGEMHistoManager {
public:

	NeutronGEMHistoManager();
	~NeutronGEMHistoManager();

	void book();
	void save();
	void fillTree(G4int* counters);
	void fillHistogram(G4int number, G4double value);

	void Fill3DEnergyElectrons(G4int id, G4double xbin, G4double ybin,
			G4double zbin, G4double weight);
	void Fill2DPositionArrivalDrift(G4int id, G4double xbin, G4double ybin,
			G4double weight = 1);

	void EndOfRun();

	void BeginOfEvent();
	void EndOfEvent();

	void AddClustersConversionElectrons();
	void AddClustersAlphas();
	void AddClustersLiIons();
	void AddClustersGammas();
	void AddClustersOtherElectrons();

	void AddEnergyConversionElectrons(G4double edep);
	void AddEnergyAlphas(G4double edep);
	void AddEnergyLiIons(G4double edep);
	void AddEnergyGammas(G4double edep);
	void AddEnergyOtherElectrons(G4double edep);

private:

	TFile* rootFile;
	TTree* fTree;
	TH1D* fHisto[NUM_HISTOGRAMS];
	TH3D* histo3DEnergyElectrons[NUM_HISTOGRAMS_3D];
	TH2D* histo2DPositionArrivalDrift[NUM_HISTOGRAMS_2D];

	G4int NeutronCapture;
	G4int ConversionElectronsCreatedConverter;
	G4int AlphasCreatedConverter;
	G4int LiIonCreatedConverter;
	G4int GammasCreatedConverter;
	G4int OtherElectronsCreatedConverter;
	G4int ConversionElectronsDrift;
	G4int AlphasDrift;
	G4int LiIonsDrift;
	G4int GammasDrift;
	G4int OtherElectronsDrift;
	G4int ElectronsCreatedDriftByCE;
	G4int ElectronsCreatedDriftByAlphas;
	G4int ElectronsCreatedDriftByLiIons;
	G4int ElectronsCreatedDriftByGammas;
	G4int ElectronsCreatedDriftByOtherElectrons;

	G4int EventsConversionElectronsCreatedConverter;
	G4int EventsAlphasCreatedConverter;
	G4int EventsLiIonCreatedConverter;
	G4int EventsGammasCreatedConverter;
	G4int EventsOtherElectronsCreatedConverter;
	G4int EventsConversionElectronsDrift;
	G4int EventsAlphasDrift;
	G4int EventsLiIonsDrift;
	G4int EventsGammasDrift;
	G4int EventsOtherElectronsDrift;
	G4int EventsElectronsCreatedDriftByCE;
	G4int EventsElectronsCreatedDriftByAlphas;
	G4int EventsElectronsCreatedDriftByLiIons;
	G4int EventsElectronsCreatedDriftByGammas;
	G4int EventsElectronsCreatedDriftByOtherElectrons;

	char fStartTime[50];
	char fEndTime[50];

	G4double fEdepConversionElectrons;
	G4double fEdepAlphas;
	G4double fEdepLiIons;
	G4double fEdepGammas;
	G4double fEdepOtherElectrons;

	G4double fClustersConversionElectrons;
	G4double fClustersAlphas;
	G4double fClustersLiIons;
	G4double fClustersGammas;
	G4double fClustersOtherElectrons;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

