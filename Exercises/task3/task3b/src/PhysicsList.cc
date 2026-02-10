PhysicsList::PhysicsList():  G4VUserPhysicsList()
{
  defaultCutValue = 1.0*mm;
  SetVerboseLevel(1);
}

PhysicsList::~PhysicsList()
{}

void PhysicsList::ConstructParticle()
{
  G4Electron::ElectronDefinition();
  G4Positron::PositronDefinition();
  G4NeutrinoE::NeutrinoEDefinition();
  G4AntiNeutrinoE::AntiNeutrinoEDefinition();
  G4NeutrinoMu::NeutrinoMuDefinition();
  G4AntiNeutrinoMu::AntiNeutrinoMuDefinition();
  G4Gamma::Gamma();
  G4MuonPlus::MuonPlusDefinition();
  G4MuonMinus::MuonMinusDefinition();
  G4PionPlus::PionPlusDefinition();
  G4PionMinus::PionMinusDefinition();
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
  //ConstructDecay();
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
      pmanager->AddDiscreteProcess(new G4PhotoElectricEffect);
      pmanager->AddDiscreteProcess(new G4ComptonScattering);
      pmanager->AddDiscreteProcess(new G4GammaConversion);
      
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
      pmanager->AddProcess(new G4MuMultipleScattering, -1, 1, 1);
      pmanager->AddProcess(new G4DecayWithSpin,         1,-1, 2);
      pmanager->AddProcess(new G4MuIonisation,         -1, 2, 3);
      pmanager->AddProcess(new G4MuBremsstrahlung,     -1, 3, 4);
      pmanager->AddProcess(new G4MuPairProduction,     -1,-1, 5);
      // ---       
    } else if (particleName == "pi-" || particleName == "pi+") {
      //pion  
      pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
      pmanager->AddProcess(new G4Decay,                1,-1, 2);
      pmanager->AddProcess(new G4hIonisation,         -1, 2, 3);
      pmanager->AddProcess(new G4hBremsstrahlung,     -1, 3, 4);
      pmanager->AddProcess(new G4hPairProduction,     -1, 4, 5);       
     
    }
  }
}

void PhysicsList::ConstructDecay()
{
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
  SetCutsWithDefault();
    
  if (verboseLevel>0) DumpCutValuesTable();
}

