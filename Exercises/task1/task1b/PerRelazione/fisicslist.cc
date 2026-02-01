void PhysicsList::ConstructParticle()
{
  // In this method, static member functions should be called
  // for all particles which you want to use.
  // This ensures that objects of these particle types will be
  // created in the program. 

  // A priori sono abilitate solamente due pseudoparticelle:
  // Il geantino neutro e il geantino carico

  // pseudo-particles
  G4Geantino::GeantinoDefinition();
  G4ChargedGeantino::ChargedGeantinoDefinition();

  /*
// gamma
  G4Gamma::Gamma();

// leptons
  G4Electron::Electron();
  G4Positron::Positron();
  */
  G4MuonPlus::MuonPlus();
  G4MuonMinus::MuonMinus();
  

  // mesons
  G4PionPlus::PionPlusDefinition();
  G4PionMinus::PionMinusDefinition();
  emPhysicsList->ConstructParticle();

}