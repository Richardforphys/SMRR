G4VPrimaryGenerator* PrimaryGeneratorAction::InitializeGPS()
{
  G4GeneralParticleSource * gps = new G4GeneralParticleSource();
  
  // setup details easier via UI commands see gps.mac

  // particle type
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* pion = particleTable->FindParticle("pi+");  
  gps->GetCurrentSource()->SetParticleDefinition(pion);

  // set energy distribution
  G4SPSEneDistribution *eneDist = gps->GetCurrentSource()->GetEneDist() ;
  eneDist->SetEnergyDisType("Mono"); // or gauss
  eneDist->SetMonoEnergy(2.0*GeV);

  // set position distribution
  G4SPSPosDistribution *posDist = gps->GetCurrentSource()->GetPosDist();
  posDist->SetPosDisType("Beam");  // or Point,Plane,Volume,Beam
  posDist->SetCentreCoords(G4ThreeVector(0.0*cm,0.0*cm,-80.0*cm));
  posDist->SetBeamSigmaInX(0.1*mm);
  posDist->SetBeamSigmaInY(0.1*mm);

  // set angular distribution
  G4SPSAngDistribution *angDist = gps->GetCurrentSource()->GetAngDist();
  angDist->SetParticleMomentumDirection( G4ThreeVector(0., 0., 1.) );
  angDist->SetAngDistType("beam2d");
  angDist->SetBeamSigmaInAngX(0.1*mrad);
  angDist->SetBeamSigmaInAngY(0.1*mrad);
  angDist->DefineAngRefAxes("angref1",G4ThreeVector(-1.,0.,0.));

  return gps;
}