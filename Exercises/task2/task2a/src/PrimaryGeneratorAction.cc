// $Id: PrimaryGeneratorAction.cc 284 2010-02-14 23:32:38Z selles $
/**
 * @file
 * @brief implements mandatory user class PrimaryGeneratorAction
 */

#include "PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4UnitsTable.hh"
#include "Randomize.hh"


PrimaryGeneratorAction::PrimaryGeneratorAction()
  : outfile(0)
{
  gun  = new G4ParticleGun(1);

  // complete particle name and energy  (do not forget the energy unit)
  G4ParticleDefinition* particle
           = G4ParticleTable::GetParticleTable()->FindParticle(...);
  gun->SetParticleDefinition(particle);
  gun->SetParticleEnergy(...);  
  
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete gun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{ 
  //this function is called to generate each G4 event 

  // Ex 2a-1 : generate only one particule

  G4double x0 = 0.*cm, y0 = 0.*cm, z0= 0.0*cm;
  G4cout<<"GeneratePrimaries : new event "<<G4BestUnit(G4ThreeVector(x0,y0,z0),"Length")<<G4endl;

  gun->SetParticlePosition(G4ThreeVector(x0,y0,z0));
  gun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  gun->GeneratePrimaryVertex(anEvent);

  // ----------------------------------------
  // Ex 2a-2 - to generate a set of particules according to a 0.1 x 2 cm rectangle 
  /*
  G4double z0 = 0.*cm, x0 = 0.*cm, y0 = 0.*cm;
  x0 = -0.05 + 2*0.05*G4UniformRand();
  y0 = -1.0+ 2*G4UniformRand();
  gun->SetParticlePosition(G4ThreeVector(x0,y0,z0));  
  gun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  gun->GeneratePrimaryVertex(anEvent);
  */

}

