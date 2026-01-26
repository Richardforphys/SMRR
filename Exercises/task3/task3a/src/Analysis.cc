// $Id: Analysis.cc 94 2010-01-26 13:18:30Z adotti $
/**
 * @file
 * @brief implements singleton class Analysis 
 */

#include "Analysis.hh"
#include "G4UnitsTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4Event.hh"
#include "G4Run.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"

#ifdef G4ANALYSIS_USE_ROOT
  #include "TROOT.h"
  #include "TFile.h"
  #include "TH1D.h"
#endif

Analysis* Analysis::singleton = 0;

Analysis* Analysis::GetInstance() {
  if ( singleton == 0 ) {
    static Analysis analysis;
    singleton = &analysis;
  }
  return singleton;
}
	
Analysis::~Analysis() 
{}

Analysis::Analysis() 
{
  m_ROOT_file = 0;
}

void Analysis::PrepareNewEvent(const G4Event* /*anEvent*/)
{
  //Reset variables relative to this event
  thisEventTotEM = 0;
  thisEventCentralEM = 0;
  thisEventSecondaries = 0;
}

void Analysis::PrepareNewRun(const G4Run* /*aRun*/ )
{
  //Reset variables relative to the run
  thisRunTotEM  = 0;
  thisRunTotEM2 = 0;
  thisRunCentralEM  = 0;
  thisRunCentralEM2 = 0;

  n_gamma = 0;
  n_electron = 0;
  n_positron = 0;

#ifdef G4ANALYSIS_USE_ROOT

  // create ROOT file
  m_ROOT_file = new TFile("task3.root","RECREATE","ROOT file with histograms");
  if(m_ROOT_file) {
    G4cout << "ROOT file task3.root is created " << G4endl;
  } else {
    G4Exception("ROOT file task3.root has not been created!");
  }

  // create histograms
  m_ROOT_histo0 = new TH1D("etot","Total energy deposit normalized to beam energy",100,0,1);
  m_ROOT_histo1 = new TH1D("e0","Energy deposit in central crystal normalized to beam energy",100,0,1);
  m_ROOT_histo2 = new TH1D("ez","Energy profile along the calorimeter (mm)",46,0,230);

#endif
}

void Analysis::EndOfEvent(const G4Event* /*anEvent*/)
{
  //Accumulate over the run
  thisRunTotEM += thisEventTotEM;
  thisRunTotEM2 += thisEventTotEM*thisEventTotEM;
  thisRunCentralEM += thisEventCentralEM;
  thisRunCentralEM2 += thisEventCentralEM*thisEventCentralEM;
  
  //Uncomment these lines for more verbosity:
  //G4cout<<"Event: "<< anEvent->GetEventID() <<" Energy in EM calo: "
  //<<G4BestUnit(thisEventTotEM,"Energy")<<" Secondaries: "<<thisEventSecondaries<<G4endl;
  //G4cout<<G4BestUnit(thisEventCentralEM,"Energy")<<G4endl;

  // save information to ROOT
#ifdef G4ANALYSIS_USE_ROOT
  m_ROOT_histo0->Fill(thisEventTotEM/beamEnergy, 1.0);
  m_ROOT_histo1->Fill(thisEventCentralEM/beamEnergy, 1.0);
#endif

}

void Analysis::EndOfRun(const G4Run* aRun)
{
  //Some print outs
  G4int numEvents = aRun->GetNumberOfEvent();
  if(numEvents == 0) { return; }

  G4double norm = numEvents*beamEnergy;
  G4cout<<"================="<<G4endl;
  G4cout<<"Summary for run: "<<aRun->GetRunID()<<G4endl;
  G4cout<<"  Beam of " << beamParticle->GetParticleName() 
	<< " kinetic energy: "<<G4BestUnit(beamEnergy,"Energy")<<G4endl;
  G4cout<<"  Event processed:         "<<numEvents<<G4endl;
  G4cout<<"  Average number of gamma: "<<(G4double)n_gamma/(G4double)numEvents<<G4endl;
  G4cout<<"  Average number of e-   : "<<(G4double)n_electron/(G4double)numEvents<<G4endl;
  G4cout<<"  Average number of e+   : "<<(G4double)n_positron/(G4double)numEvents<<G4endl;
  G4cout<<"  Average energy deposition in EM calo: "
	<<G4BestUnit(thisRunTotEM/(G4double)numEvents,"Energy")<<G4endl;
  G4cout<<"  Normalized energy in EM calo:         "<<thisRunTotEM/norm;
  G4double rms = std::sqrt(thisRunTotEM2*numEvents - thisRunTotEM*thisRunTotEM)/norm;
  G4cout<<"  RMS: "<<rms<<G4endl;
  G4cout<<"  Normalized energy in central crystal: "<<thisRunCentralEM/norm;
  rms = std::sqrt(thisRunCentralEM2*numEvents - thisRunCentralEM*thisRunCentralEM)/norm;
  G4cout<<"  RMS: "<<rms<<G4endl;
  G4cout<<"  Ratio of central crystal to total:    "<<thisRunCentralEM/thisRunTotEM
	<<G4endl;
  G4cout<<"================="<<G4endl;

  // Writing and closing the ROOT file
#ifdef G4ANALYSIS_USE_ROOT
  G4cout << "ROOT: files writing..." << G4endl;
  m_ROOT_file->Write();
  G4cout << "ROOT: files closing..." << G4endl;
  m_ROOT_file->Close();
  delete m_ROOT_file;
#endif

}

void Analysis::AddSecondary(const G4ParticleDefinition* part)
{
  if(part == G4Gamma::Gamma()) { ++n_gamma; }
  else if(part == G4Electron::Electron()) { ++n_electron; }  
  else if(part == G4Positron::Positron()) { ++n_positron; }
}

void Analysis::AddEDepEM(G4double edep, G4double z, G4int copyno)
{
  thisEventTotEM += edep;
  if(11 == copyno && z > -DBL_MAX) { thisEventCentralEM += edep; }
#ifdef G4ANALYSIS_USE_ROOT
  m_ROOT_histo2->Fill(z - eCalZposition, edep);
#endif
}

void Analysis::SetBeam(const G4ParticleDefinition* part, G4double energy)
{
  beamParticle = part;
  beamEnergy = energy;
}
