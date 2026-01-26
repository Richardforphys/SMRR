// $Id: Analysis.hh 94 2010-01-26 13:18:30Z adotti $
/**
 * @file
 * @brief Analysis of calorimeter application.
 */

/*
 * Analysis.hh
 *
 *  Created on: 9 Feb 2010
 *      Author: adotti
 */

#ifndef ANALYSIS_HH
#define ANALYSIS_HH 1

#include "globals.hh"
#include <vector>

class G4Run;
class G4Event;
class G4ParticleDefinition;
class TH1;

class Analysis {

public:

  static Analysis* GetInstance();
  ~Analysis();

  void PrepareNewEvent(const G4Event* anEvent);
  void EndOfEvent(const G4Event* anEvent);
  void PrepareNewRun(const G4Run* aRun);
  void EndOfRun(const G4Run* aRun);
  void AddSecondary(const G4ParticleDefinition* part);
  void AddEDepEM(G4double edep, G4double z, G4int copyno);
  void SetEcalZposition(G4double val) { eCalZposition = val; };
  void SetBeam(const G4ParticleDefinition* part, G4double energy);

private:

  Analysis();
  static Analysis* singleton;

  // beam and calorimeter geometry
  const G4ParticleDefinition* beamParticle;
  G4double beamEnergy;
  G4double eCalZposition;

  // simple analysis parameters
  G4double thisEventTotEM;
  G4double thisEventCentralEM;
  G4double thisRunTotEM;
  G4double thisRunTotEM2;
  G4double thisRunCentralEM;
  G4double thisRunCentralEM2;

  // counters
  G4int thisEventSecondaries;
  G4int n_gamma;
  G4int n_electron;
  G4int n_positron;

  // ROOT objects
  /*
  TFile*    m_ROOT_file;
  TH1D*     m_ROOT_histo0;
  TH1D*     m_ROOT_histo1;
  TH1D*     m_ROOT_histo2;
  */
  std::vector<TH1*> histos;
  enum {
    fEnergyTotal=0,
    fEnergyCentral=1,
    fEnergyProfile=2
  };

};

#endif /* ANALYSIS_HH */
