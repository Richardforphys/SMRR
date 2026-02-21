#include "NeutronGEMHistoManager.hh"
#include "NeutronGEMDataManager.hh"
#include "G4UnitsTable.hh"
#include "G4Step.hh"
#include "G4LossTableManager.hh"
#include "G4PhysicalConstants.hh"

#include <iomanip>

#include "TH1D.h"
#include "TH3D.h"
#include "TH2D.h"
#include "TBranch.h"
#include "TFile.h"
#include "TTree.h"
#include "G4SystemOfUnits.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NeutronGEMHistoManager::NeutronGEMHistoManager() :
		rootFile(0), fTree(0) {

	BeginOfEvent();

	for (G4int k = 0; k < NUM_HISTOGRAMS; k++)
		fHisto[k] = 0;
	for (G4int k = 0; k < NUM_HISTOGRAMS_2D; k++)
		histo2DPositionArrivalDrift[k] = 0;
	for (G4int k = 0; k < NUM_HISTOGRAMS_3D; k++)
		histo3DEnergyElectrons[k] = 0;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NeutronGEMHistoManager::~NeutronGEMHistoManager() {
#ifdef G4ANALYSIS_USE  
	/*
	 if (fTree) {
	 delete fTree;
	 fTree = 0;
	 }
	 for (G4int k = 0; k < NUM_HISTOGRAMS; k++)
	 {
	 if(fHisto[k]) delete fHisto[k];
	 }
	 if (histo3DEnergyElectronsCreatedByGamma) {
	 delete histo3DEnergyElectronsCreatedByGamma;
	 histo3DEnergyElectronsCreatedByGamma = 0;
	 }
	 if (histo3DEnergyElectronsCreatedByElectron) {
	 delete histo3DEnergyElectronsCreatedByElectron;
	 histo3DEnergyElectronsCreatedByElectron = 0;
	 }
	 */
	if (rootFile) {
		delete rootFile;
		rootFile = 0;
	}
	G4cout << "Deconstructor NeutronGEMHistoManager" << G4endl;
#endif    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NeutronGEMHistoManager::book() {
#ifdef G4ANALYSIS_USE

	// Creating a tree container to handle histograms and ntuples.
	// This tree is associated to an output file.
	//

	time_t rawtime;
	struct tm * timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(fStartTime, 50, "%Y-%m-%d-%H-%M-%S", timeinfo);
	NeutronGEMDataManager* dataManager = NeutronGEMDataManager::GetInstance();
	G4int numEvents = dataManager->getNumberOfEvents();
	std::ostringstream sEvents;
	sEvents << numEvents;
	G4String fileName = dataManager->getDescription() + "_" + sEvents.str()
			+ "_" + fStartTime + ".root";

	rootFile = new TFile(fileName, "RECREATE");
	if (!rootFile) {
		G4cout << " HistoManager::book :" << " problem creating the ROOT TFile "
				<< G4endl;
		return;
	}

	fTree = new TTree("Gd GEM", "Gd GEM");

	fTree->Branch("NeutronCapture", &NeutronCapture, "NeutronCapture/I");

	fTree->Branch("ConversionElectronsCreatedConverter",
			&ConversionElectronsCreatedConverter,
			"ConversionElectronsCreatedConverter/I");
	fTree->Branch("AlphasCreatedConverter", &AlphasCreatedConverter,
			"AlphasCreatedConverter/I");
	fTree->Branch("LiIonCreatedConverter", &LiIonCreatedConverter,
			"LiIonCreatedConverter/I");
	fTree->Branch("GammasCreatedConverter", &GammasCreatedConverter,
			"GammasCreatedConverter/I");
	fTree->Branch("OtherElectronsCreatedConverter",
			&OtherElectronsCreatedConverter,
			"OtherElectronsCreatedConverter/I");

	fTree->Branch("ConversionElectronsDrift", &ConversionElectronsDrift,
			"ConversionElectronsDrift/I");
	fTree->Branch("AlphasDrift", &AlphasDrift, "AlphasDrift/I");
	fTree->Branch("LiIonsDrift", &LiIonsDrift, "LiIonsDrift/I");
	fTree->Branch("GammasDrift", &GammasDrift, "GammasDrift/I");
	fTree->Branch("OtherElectronsDrift", &OtherElectronsDrift,
			"OtherElectronsDrift/I");

	fTree->Branch("ElectronsCreatedDriftByCE", &ElectronsCreatedDriftByCE,
			"ElectronsCreatedDriftByCE/I");
	fTree->Branch("ElectronsCreatedDriftByAlphas",
			&ElectronsCreatedDriftByAlphas, "ElectronsCreatedDriftByAlphas/I");
	fTree->Branch("ElectronsCreatedDriftByLiIons",
			&ElectronsCreatedDriftByLiIons, "ElectronsCreatedDriftByLiIons/I");
	fTree->Branch("ElectronsCreatedDriftByGammas",
			&ElectronsCreatedDriftByGammas, "ElectronsCreatedDriftByGammas/I");
	fTree->Branch("ElectronsCreatedDriftByOtherElectrons",
			&ElectronsCreatedDriftByOtherElectrons,
			"ElectronsCreatedDriftByOtherElectrons/I");

	fTree->Branch("EventsConversionElectronsCreatedConverter",
			&EventsConversionElectronsCreatedConverter,
			"EventsConversionElectronsCreatedConverter/I");
	fTree->Branch("EventsAlphasCreatedConverter", &EventsAlphasCreatedConverter,
			"EventsAlphasCreatedConverter/I");
	fTree->Branch("EventsLiIonCreatedConverter", &EventsLiIonCreatedConverter,
			"EventsLiIonCreatedConverter/I");
	fTree->Branch("EventsGammasCreatedConverter", &EventsGammasCreatedConverter,
			"EventsGammasCreatedConverter/I");
	fTree->Branch("EventsOtherElectronsCreatedConverter",
			&EventsOtherElectronsCreatedConverter,
			"EventsOtherElectronsCreatedConverter/I");

	fTree->Branch("EventsConversionElectronsDrift",
			&EventsConversionElectronsDrift,
			"EventsConversionElectronsDrift/I");
	fTree->Branch("EventsAlphasDrift", &EventsAlphasDrift,
			"EventsAlphasDrift/I");
	fTree->Branch("EventsLiIonsDrift", &EventsLiIonsDrift,
			"EventsLiIonsDrift/I");
	fTree->Branch("EventsGammasDrift", &EventsGammasDrift,
			"EventsGammasDrift/I");
	fTree->Branch("EventsOtherElectronsDrift", &EventsOtherElectronsDrift,
			"EventsOtherElectronsDrift/I");

	fTree->Branch("EventsElectronsCreatedDriftByCE",
			&EventsElectronsCreatedDriftByCE,
			"EventsElectronsCreatedDriftByCE/I");
	fTree->Branch("EventsElectronsCreatedDriftByAlphas",
			&EventsElectronsCreatedDriftByAlphas,
			"EventsElectronsCreatedDriftByAlphas/I");
	fTree->Branch("EventsElectronsCreatedDriftByLiIons",
			&EventsElectronsCreatedDriftByLiIons,
			"EventsElectronsCreatedDriftByLiIons/I");
	fTree->Branch("EventsElectronsCreatedDriftByGammas",
			&EventsElectronsCreatedDriftByGammas,
			"EventsElectronsCreatedDriftByGammas/I");
	fTree->Branch("EventsElectronsCreatedDriftByOtherElectrons",
			&EventsElectronsCreatedDriftByOtherElectrons,
			"EventsElectronsCreatedDriftByOtherElectrons/I");

	fHisto[1] = new TH1D("1", "EnergyConversionElectronsCreatedConverter", 1000,
			0., 250 * CLHEP::keV);

	fHisto[2] = new TH1D("2", "EnergyAlphasCreatedConverter", 1000, 0.,
			2000 * CLHEP::keV);

	fHisto[3] = new TH1D("3", "EnergyLiIonsCreatedConverter", 1000, 0.,
			2000 * CLHEP::keV);
	fHisto[4] = new TH1D("4", "EnergyGammasCreatedConverter", 1000, 0.,
			6000 * CLHEP::keV);

	fHisto[5] = new TH1D("5", "EnergyOtherElectronsCreatedConverter", 1000, 0.,
			1000 * CLHEP::keV);

	fHisto[6] = new TH1D("6", "EnergyConversionElectronsDrift", 1000, 0.,
			250 * CLHEP::keV);

	fHisto[7] = new TH1D("7", "EnergyAlphasDrift", 1000, 0., 2000 * CLHEP::keV);

	fHisto[8] = new TH1D("8", "EnergyLiIonsDrift", 1000, 0., 2000 * CLHEP::keV);
	fHisto[9] = new TH1D("9", "EnergyGammasDrift", 1000, 0., 6000 * CLHEP::keV);
	fHisto[10] = new TH1D("10", "EnergyOtherElectronsDrift", 1000, 0.,
			1000 * CLHEP::keV);

	fHisto[11] = new TH1D("11", "EnergyElectronsCreatedDriftByCE", 10000, 0.,
			100);
	fHisto[12] = new TH1D("12", "EnergyElectronsCreatedDriftByAlphas", 10000,
			0., 100);
	fHisto[13] = new TH1D("13", "EnergyElectronsCreatedDriftByLiIons", 10000,
			0., 100);
	fHisto[14] = new TH1D("14", "EnergyElectronsCreatedDriftByGammas", 10000,
			0., 100);
	fHisto[15] = new TH1D("15", "EnergyElectronsCreatedDriftByOtherElectrons",
			10000, 0., 100);

	fHisto[16] = new TH1D("16",
			"Distance from (0,0) conversion electrons drift", 1000, 0., 8);
	fHisto[17] = new TH1D("17", "Distance from (0,0) alphas drift", 1000, 0.,
			8);
	fHisto[18] = new TH1D("18", "Distance from (0,0) Li ions drift", 1000, 0.,
			8);
	fHisto[19] = new TH1D("19", "Distance from (0,0) gammas drift", 1000, 0.,
			8);
	fHisto[20] = new TH1D("20", "Distance from (0,0) other electrons drift",
			1000, 0., 8);

	fHisto[21] = new TH1D("21", "Deposited energy conversion electrons gas",
			1000, 0., 100 * CLHEP::keV);

	fHisto[22] = new TH1D("22", "Deposited energy alpha gas", 1000, 0.,
			2000 * CLHEP::keV);
	fHisto[23] = new TH1D("23", "Deposited energy Li ions gas", 1000, 0.,
			2000 * CLHEP::keV);
	fHisto[24] = new TH1D("24", "Deposited energy gammas gas", 1000, 0.,
			100 * CLHEP::keV);
	fHisto[25] = new TH1D("25", "Deposited energy other electrons gas", 1000,
			0., 100 * CLHEP::keV);

	fHisto[26] = new TH1D("26", "Number of clusters conversion electrons gas",
			1000, 0., 1000);

	fHisto[27] = new TH1D("27", "Number of clusters alphas gas", 1000, 0.,
			1000);

	fHisto[28] = new TH1D("28", "Number of clusters Li ions gas", 1000, 0.,
			1000);

	fHisto[29] = new TH1D("29", "Number of clusters gammas gas", 1000, 0.,
			1000);
	fHisto[30] = new TH1D("30", "Number of clusters other electrons gas", 1000,
			0., 1000);

	histo3DEnergyElectrons[1] = new TH3D("Energy e- created by ce",
			"Energy e- created by ce", 100, -5., 5., 100, -5., 5, 100, -0.5,
			0.5);

	histo3DEnergyElectrons[2] = new TH3D("Energy e- created by alphas",
			"Energy e- created by ce", 100, -5., 5., 100, -5., 5, 100, -0.5,
			0.5);

	histo3DEnergyElectrons[3] = new TH3D("Energy e- created by Li ions",
			"Energy e- created by ce", 100, -5., 5., 100, -5., 5, 100, -0.5,
			0.5);

	histo3DEnergyElectrons[4] = new TH3D("Energy e- created by gammas",
			"Energy e- created by prompt gamma", 100, -5., 5., 100, -5., 5, 100,
			-0.5, 0.5);

	histo3DEnergyElectrons[5] = new TH3D("Energy e- created by other electrons",
			"Energy e- created by other electrons", 100, -5., 5., 100, -5., 5,
			100, -0.5, 0.5);

	histo2DPositionArrivalDrift[1] = new TH2D(
			"Position arrival e- created by ce drift",
			"Position arrival e- created by ce drift", 100, -5., 5., 100, -5.,
			5);

	histo2DPositionArrivalDrift[2] = new TH2D(
			"Position arrival alpha drift", "Position arrival alpha drift", 100,
			-5., 5., 100, -5., 5);

	histo2DPositionArrivalDrift[3] = new TH2D(
			"Position arrival Li ion drift", "Position arrival Li ion drift",
			100, -5., 5., 100, -5., 5);
	histo2DPositionArrivalDrift[4] = new TH2D("Position arrival gamma",
			"Position arrival gamma drift", 100, -5., 5., 100, -5., 5);

	histo2DPositionArrivalDrift[5] = new TH2D(
			"Position arrival other electrons drift",
			"Position arrival other electrons drift", 100, -5., 5., 100, -5.,
			5);

	G4cout << "\n----> Histogram file is opened in " << fileName << G4endl;
#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NeutronGEMHistoManager::save() {
#ifdef G4ANALYSIS_USE

	if (rootFile) {
		rootFile->Write();       // Writing the histograms to the file
		rootFile->Close();    // and closing the tree (and the file)
		G4cout << "\n----> Histogram Tree is saved \n" << G4endl;
	}
#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NeutronGEMHistoManager::fillTree(G4int* counters) {

	NeutronCapture = counters[0];

	ConversionElectronsCreatedConverter = counters[1];
		AlphasCreatedConverter = counters[2];
		LiIonCreatedConverter = counters[3];
		GammasCreatedConverter = counters[4];
		OtherElectronsCreatedConverter = counters[5];
		ConversionElectronsDrift = counters[6];
		AlphasDrift = counters[7];
		LiIonsDrift = counters[8];
		GammasDrift = counters[9];
		OtherElectronsDrift = counters[10];
		ElectronsCreatedDriftByCE = counters[11];
		ElectronsCreatedDriftByAlphas = counters[12];
		ElectronsCreatedDriftByLiIons = counters[13];
		ElectronsCreatedDriftByGammas = counters[14];
		ElectronsCreatedDriftByOtherElectrons = counters[15];

		EventsConversionElectronsCreatedConverter = counters[16];
		EventsAlphasCreatedConverter = counters[17];
		EventsLiIonCreatedConverter = counters[18];
		EventsGammasCreatedConverter = counters[19];
		EventsOtherElectronsCreatedConverter = counters[20];
		EventsConversionElectronsDrift = counters[21];
		EventsAlphasDrift = counters[22];
		EventsLiIonsDrift = counters[23];
		EventsGammasDrift = counters[24];
		EventsOtherElectronsDrift = counters[25];
		EventsElectronsCreatedDriftByCE = counters[26];
		EventsElectronsCreatedDriftByAlphas = counters[27];
		EventsElectronsCreatedDriftByLiIons = counters[28];
		EventsElectronsCreatedDriftByGammas = counters[29];
		EventsElectronsCreatedDriftByOtherElectrons = counters[30];


	if (fTree)
		fTree->Fill();

}

void NeutronGEMHistoManager::fillHistogram(G4int number, G4double value) {
	fHisto[number]->Fill(value);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NeutronGEMHistoManager::Fill3DEnergyElectrons(G4int id,
		G4double xbin, G4double ybin, G4double zbin, G4double weight) {
#ifdef G4ANALYSIS_USE
	if (histo3DEnergyElectrons[id]) {
		histo3DEnergyElectrons[id]->Fill(xbin, ybin, zbin, weight);
	}
#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NeutronGEMHistoManager::Fill2DPositionArrivalDrift(G4int id, G4double xbin,
		G4double ybin, G4double weight) {
#ifdef G4ANALYSIS_USE
	if (histo2DPositionArrivalDrift[id]) {
		histo2DPositionArrivalDrift[id]->Fill(xbin, ybin, weight);
	}
#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void NeutronGEMHistoManager::BeginOfEvent() {
	fEdepConversionElectrons = 0.0;
	fEdepAlphas = 0.0;
	fEdepLiIons = 0.0;
	fEdepGammas = 0.0;
	fEdepOtherElectrons = 0.0;

	fClustersConversionElectrons = 0.0;
	fClustersAlphas = 0.0;
	fClustersLiIons = 0.0;
	fClustersGammas = 0.0;
	fClustersOtherElectrons = 0.0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void NeutronGEMHistoManager::EndOfEvent() {
	if (fEdepConversionElectrons > 0)
		fillHistogram(21, fEdepConversionElectrons);
	if (fEdepAlphas > 0)
		fillHistogram(22, fEdepAlphas);
	if (fEdepLiIons > 0)
		fillHistogram(23, fEdepLiIons);
	if (fEdepGammas > 0)
		fillHistogram(24, fEdepGammas);
	if (fEdepOtherElectrons > 0)
		fillHistogram(25, fEdepOtherElectrons);

	if (fClustersConversionElectrons > 0)
		fillHistogram(26, fClustersConversionElectrons);
	if (fClustersAlphas > 0)
		fillHistogram(27, fClustersAlphas);
	if (fClustersLiIons > 0)
		fillHistogram(28, fClustersLiIons);
	if (fClustersGammas > 0)
		fillHistogram(29, fClustersGammas);
	if (fClustersOtherElectrons > 0)
		fillHistogram(30, fClustersOtherElectrons);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NeutronGEMHistoManager::AddEnergyConversionElectrons(G4double edep) {
	fEdepConversionElectrons += edep;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NeutronGEMHistoManager::AddEnergyGammas(G4double edep) {
	fEdepGammas += edep;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NeutronGEMHistoManager::AddEnergyAlphas(G4double edep) {
	fEdepAlphas += edep;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NeutronGEMHistoManager::AddEnergyLiIons(G4double edep) {
	fEdepLiIons += edep;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NeutronGEMHistoManager::AddEnergyOtherElectrons(G4double edep) {
	fEdepOtherElectrons += edep;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NeutronGEMHistoManager::AddClustersConversionElectrons() {
	fClustersConversionElectrons++;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NeutronGEMHistoManager::AddClustersAlphas() {
	fClustersAlphas++;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void NeutronGEMHistoManager::AddClustersLiIons() {
	fClustersLiIons++;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NeutronGEMHistoManager::AddClustersGammas() {
	fClustersGammas++;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void NeutronGEMHistoManager::AddClustersOtherElectrons() {
	fClustersOtherElectrons++;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
