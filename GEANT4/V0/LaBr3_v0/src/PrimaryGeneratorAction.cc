#include "PrimaryGeneratorAction.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
//Way of initializing the variable "outfile" to 0.
    : outfile(0) {
        gun = InitializeGPS(); 
//When instanziating a PrimaryGeneratorAction object, a gun variable will we defined, via the InuitializeGPS method
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {
    delete gun;
}

G4VPrimaryGenerator* PrimaryGeneratorAction::InitializeGPS() {
    //Define the particle generator
    G4GeneralParticleSource* gps = new G4GeneralParticleSource(); 
    //The InitializeGPS method, creates a pointer to a G4GeneralParticleSource object, called gps

    //Particle type
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* gamma = particleTable->FindParticle("gamma");
    gps->GetCurrentSource()->SetParticleDefinition(gamma);

    //Set energy distribution
    G4SPSEneDistribution* eneDist = gps->GetCurrentSource()->GetEneDist();
    eneDist->SetEnergyDisType("Mono");
    eneDist->SetMonoEnergy(1.0*MeV);

    //Set poisition distribution
    G4SPSPosDistribution* posDist = gps->GetCurrentSource()->GetPosDist();
    posDist->SetPosDisType("Beam");
    posDist->SetCentreCoords(G4ThreeVector(0.0*cm, 0.0*cm, -5.0*m));
    posDist->SetBeamSigmaInX(0.1*mm);
    posDist->SetBeamSigmaInY(0.1*mm);

    //Set Angular Distribution
    G4SPSAngDistribution* angDist = gps->GetCurrentSource()->GetAngDist();
    angDist->SetParticleMomentumDirection(G4ThreeVector(0.0, 0.0, 1.0));
    angDist->SetBeamSigmaInAngX(0.01*rad);
    angDist->SetBeamSigmaInAngY(0.01*rad);

    return gps;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
    gun->GeneratePrimaryVertex(anEvent);
}