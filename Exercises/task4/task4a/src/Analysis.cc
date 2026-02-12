#include "Analysis.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "PrimaryGeneratorAction.hh"
#include "TFile.h"
#include "TH1D.h"
#include "TSystem.h"
#include <sstream>
#include <iomanip>
#include <cmath>

Analysis* Analysis::singleton = nullptr;

// --------------------------------------------------------------------
Analysis::Analysis()
  : thisEventTotEM(0),
    thisEventSecondaries(0),
    thisRunTotEM(0),
    thisRunTotSecondaries(0),
    outFile(nullptr)
{
}

// --------------------------------------------------------------------
void Analysis::PrepareNewEvent(const G4Event*)
{
    thisEventSecondaries = 0;
    thisEventTotEM = 0;
    thisEventStepLengthEM = 0;

    for (int i = 0; i < NUMLAYERS; ++i) {
        thisEventTotHad[i] = 0;
        thisEventStepLengthHad[i] = 0;
    }
}

// --------------------------------------------------------------------
void Analysis::EndOfEvent(const G4Event*)
{
    thisRunTotEM += thisEventTotEM;
    thisRunTotSecondaries += thisEventSecondaries;
    thisRunStepLengthEM += thisEventStepLengthEM;

    for (int i = 0; i < NUMLAYERS; ++i) {
        thisRunTotHad[i] += thisEventTotHad[i];
        thisRunStepLengthHad[i] += thisEventStepLengthHad[i];
    }
}

// --------------------------------------------------------------------
void Analysis::AddStepLength(G4int layer, G4double step)
{
    if (layer < 0 || layer >= (G4int)histos_stepLength.size()) return;
    histos_stepLength[layer]->Fill(step / mm);
}

// --------------------------------------------------------------------
void Analysis::PrepareNewRun(const G4Run*)
{
    // ---------- clean previous run ----------
    histos_stepLength.clear();

    // ---------- output directory ----------
    G4String save_path =
        "/home/ubuntu/SMRR/Exercises/task4/task4a_build/Analyse/";
    gSystem->mkdir(save_path, kTRUE);

    // ---------- primary energy ----------
    auto runManager = G4RunManager::GetRunManager();
    auto primary =
        static_cast<const PrimaryGeneratorAction*>(
            runManager->GetUserPrimaryGeneratorAction());

    G4double energy = primary->GetEnergy() / MeV;
	G4cout << "Primary energy: " << energy << " MeV" << G4endl;

    // ---------- output file ----------
    std::ostringstream filename;
    filename << save_path
             << "stepLength_E"
             << std::fixed << std::setprecision(0)
             << energy << "MeV.root";

    outFile = new TFile(filename.str().c_str(), "RECREATE");

    // ---------- reset run counters ----------
    thisRunTotEM = 0;
    thisRunTotSecondaries = 0;
    thisRunStepLengthEM = 0;

    for (int i = 0; i < NUMLAYERS; ++i) {
        thisRunTotHad[i] = 0;
        thisRunStepLengthHad[i] = 0;
    }

    // ---------- histogram binning ----------
    const int nBins = 80;
    const double xmin = 1e-4;   // mm
    const double xmax = 100.0;  // mm

    double logBins[nBins + 1];
    double logMin = std::log10(xmin);
    double logMax = std::log10(xmax);
    double dlog = (logMax - logMin) / nBins;

    for (int i = 0; i <= nBins; ++i)
        logBins[i] = std::pow(10., logMin + i * dlog);

    // ---------- one histogram per layer ----------
    for (int layer = 0; layer < NUMLAYERS; ++layer) {

        std::ostringstream hname;
        hname << "stepLength_layer" << layer;

        TH1D* h = new TH1D(
            hname.str().c_str(),
            "Muon step length in air;step length [mm];steps",
            nBins, logBins);

        h->StatOverflows();
        histos_stepLength.push_back(h);
    }
}

// --------------------------------------------------------------------
void Analysis::EndOfRun(const G4Run* aRun)
{
    G4int numEvents = aRun->GetNumberOfEvent();

    G4double totHadCalo = 0;
    for (int i = 0; i < NUMLAYERS; ++i)
        totHadCalo += thisRunTotHad[i];

    // ---------- write ROOT ----------
    outFile->cd();
    for (auto h : histos_stepLength) {
        h->Write();
        delete h;
    }
    histos_stepLength.clear();

    outFile->Close();
    delete outFile;
    outFile = nullptr;

    // ---------- print summary ----------
    G4cout << "=================" << G4endl;
    G4cout << "Summary for run: " << aRun->GetRunID() << G4endl;
    G4cout << "\t Events processed: " << numEvents << G4endl;
    G4cout << "\t Average secondaries: "
           << thisRunTotSecondaries / numEvents << G4endl;
    G4cout << "\t Average energy in EM calo: "
           << G4BestUnit(thisRunTotEM / numEvents, "Energy") << G4endl;
    G4cout << "\t Average energy in Had calo: "
           << G4BestUnit(totHadCalo / numEvents, "Energy") << G4endl;
    G4cout << "\t Average step length in EM calo: "
           << G4BestUnit(thisRunStepLengthEM / numEvents, "Length") << G4endl;

    for (int layer = 0; layer < NUMLAYERS; ++layer) {
        G4cout << "\t\t Layer " << layer
               << " energy: "
               << G4BestUnit(thisRunTotHad[layer] / numEvents, "Energy")
               << G4endl;
        G4cout << "\t\t Layer " << layer
               << " step length: "
               << G4BestUnit(thisRunStepLengthHad[layer] / numEvents, "Length")
               << G4endl;
    }

    G4cout << "=================" << G4endl;
}