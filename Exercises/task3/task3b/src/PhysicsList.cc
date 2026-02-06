// $Id: PhysicsList.cc 98 2010-01-26 15:59:09Z vnivanch $
/**
 * @file
 * @brief Implements mandatory user class PhysicsList.
 */

#include "globals.hh"
#include "PhysicsList.hh"
#include "G4LossTableManager.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleTypes.hh"
#include "G4DecayTable.hh"
#include "G4Decay.hh"
#include "G4DecayWithSpin.hh"
#include "G4MuonDecayChannelWithSpin.hh"
#include "G4MuonRadiativeDecayChannelWithSpin.hh"
#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4eMultipleScattering.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"
#include "G4EmStandardPhysics.hh"
#include "G4ParticleTable.hh"
#include "G4MuMultipleScattering.hh"
#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"
#include "G4SystemOfUnits.hh"
#include "G4hMultipleScattering.hh"
#include "G4hIonisation.hh"
#include "G4hBremsstrahlung.hh"
#include "G4hPairProduction.hh"


PhysicsList::PhysicsList():  G4VUserPhysicsList()
{
  defaultCutValue = 1.0*mm;
  SetVerboseLevel(1);
}

PhysicsList::~PhysicsList()
{}

void PhysicsList::ConstructParticle()
{
  // In this method, static member functions should be called
  // for all particles which you want to use.
  // This ensures that objects of these particle types will be
  // created in the program. 

  // pseudo-particles
  //G4Geantino::GeantinoDefinition();
  //G4ChargedGeantino::ChargedGeantinoDefinition();

  // leptons
  G4Electron::ElectronDefinition();
  G4Positron::PositronDefinition();

  // ********************************************************************************
  // Task 3.b - Exercise 4 
  //   - add particle definitions for mu+ and mu- here
  // ********************************************************************************

 
  // ---


  G4NeutrinoE::NeutrinoEDefinition();
  G4AntiNeutrinoE::AntiNeutrinoEDefinition();
  G4NeutrinoMu::NeutrinoMuDefinition();
  G4AntiNeutrinoMu::AntiNeutrinoMuDefinition();


  // gamma
  G4Gamma::Gamma();
  G4MuonPlus::MuonPlusDefinition();
  G4MuonMinus::MuonMinusDefinition();

  // mesons
  //G4PionPlus::PionPlusDefinition();
  //G4PionMinus::PionMinusDefinition();

  // ********************************************************************************
  // Task 3.b - Exercise 7 
  //   to activate polarisation
  //   -  uncomment the lines below
  //   -  change the decay process in  PhysicsList::ConstructDecay() to
  //      G4DecayWithSpin
  // ********************************************************************************
  //
  
  /*
  G4DecayTable* MuonPlusDecayTable = new G4DecayTable();
  MuonPlusDecayTable -> Insert(new G4MuonDecayChannelWithSpin("mu+",0.986));
  MuonPlusDecayTable -> Insert(new G4MuonRadiativeDecayChannelWithSpin("mu+",0.014));
  G4MuonPlus::MuonPlusDefinition() -> SetDecayTable(MuonPlusDecayTable);

  G4DecayTable* MuonMinusDecayTable = new G4DecayTable();
  MuonMinusDecayTable -> Insert(new G4MuonDecayChannelWithSpin("mu-",0.986));
  MuonMinusDecayTable -> Insert(new G4MuonRadiativeDecayChannelWithSpin("mu-",0.014));
  G4MuonMinus::MuonMinusDefinition() -> SetDecayTable(MuonMinusDecayTable);
  */
}

void PhysicsList::ConstructProcess()
{
  AddTransportation();

  ConstructDecay();
  ConstructEM();
}

void PhysicsList::ConstructEM()
{
  GetParticleIterator()->reset();
  while( (*GetParticleIterator())() ){
    G4ParticleDefinition* particle = GetParticleIterator()->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();
    
    if (particleName == "gamma") {
      // gamma         
      //pmanager->AddDiscreteProcess(new G4PhotoElectricEffect);
      //pmanager->AddDiscreteProcess(new G4ComptonScattering);
      //pmanager->AddDiscreteProcess(new G4GammaConversion);
      
    } else if (particleName == "e-") {
      //electron
      pmanager->AddProcess(new G4eMultipleScattering,-1, 1, 1);
      pmanager->AddProcess(new G4eIonisation,        -1, 2, 2);
      pmanager->AddProcess(new G4eBremsstrahlung,    -1, 3, 3);      

    } else if (particleName == "e+") {
      //positron
      pmanager->AddProcess(new G4eMultipleScattering,-1, 1, 1);
      pmanager->AddProcess(new G4eIonisation,        -1, 2, 2);
      pmanager->AddProcess(new G4eBremsstrahlung,    -1, 3, 3);
      pmanager->AddProcess(new G4eplusAnnihilation,   0,-1, 4);
    
    } else if( particleName == "mu-" || particleName =="mu+"){
      //muon  
      // ********************************************************************************
      // Task 3.b - Exercise 6 
      //  - Add the processes G4MuIonisation, G4MuBremsstrahlung, G4MuPairProduction, 
      //    and also G4MuMultipleScattering, what order is needed?
      // ********************************************************************************
      
      pmanager->AddProcess(new G4MuMultipleScattering, -1, 1, 1);
      pmanager->AddProcess(new G4MuIonisation,         -1, 2, 2);
      pmanager->AddProcess(new G4MuBremsstrahlung,     -1, 3, 3);
      pmanager->AddProcess(new G4MuPairProduction,     -1,-1,-1);
      // ---       
    } else if (particleName == "pi-" || particleName == "pi+") {
      //pion  
      pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
      pmanager->AddProcess(new G4hIonisation,         -1, 2, 2);
      pmanager->AddProcess(new G4hBremsstrahlung,     -1, 3, 3);
      pmanager->AddProcess(new G4hPairProduction,     -1, 4, 4);       
     
    }
  }
}

void PhysicsList::ConstructDecay()
{
  // ********************************************************************************
  // Task 3.b - Exercise 5 
  //   - Add a Muon decay process (both AtRest and PostStep) for mu-.
  //   - optionally add a decay also for mu+. 
  // Task 3.b - Exercise 7
  //   - Change the muon decay process to a G4DecayWithSpin. 
  // ********************************************************************************
  
  G4Decay* MuMinusDecay = new G4Decay();
  G4ParticleDefinition* muMinus= G4MuonMinus::MuonMinusDefinition();
  G4ProcessManager* muMinusManager = muMinus->GetProcessManager();
  muMinusManager->AddProcess(MuMinusDecay,      1, -1, 1);

  G4Decay* MuPlusDecay = new G4Decay();
  G4ParticleDefinition* muPlus= G4MuonPlus::MuonPlusDefinition();
  G4ProcessManager* muPlusManager = muPlus->GetProcessManager();
  muPlusManager->AddProcess(MuPlusDecay,        1, -1, 1);
 }

void PhysicsList::SetCuts()
{
  //G4VUserPhysicsList::SetCutsWithDefault method sets 
  //the default cut value for all particle types 
  //
  SetCutsWithDefault();
     
  if (verboseLevel>0) DumpCutValuesTable();
}

