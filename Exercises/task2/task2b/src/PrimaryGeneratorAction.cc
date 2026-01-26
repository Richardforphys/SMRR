/*!
\file
\brief implements mandatory user class PrimaryGeneratorAction
*/

#include "PrimaryGeneratorAction.hh"
#include "PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"

#include "G4GeneralParticleSource.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
  /*  
  G4ParticleGun * particleGun = new G4ParticleGun();
  particleGun->SetParticleEnergy(2.0*GeV);
  particleGun->SetParticlePosition(G4ThreeVector(0., 0., 0.));
  particleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  */

  // ********************************************************************************
  // Task2b - 
  //   - change the initialisation of gun from the class G4ParticleGun 
  //     to the class G4GeneralParticleSource 
  //     and set the parameters via the following UI commands 
  //       /gps/energy 2. GeV
  //       /gps/direction 0 0 1
  //       /gps/position 0 0 -1. m
  //  
  // ********************************************************************************

  G4GeneralParticleSource *gps = new G4GeneralParticleSource();
 
  gps->GetCurrentSource()->GetEneDist()->SetMonoEnergy(2.0*GeV);
  gps->GetCurrentSource()->GetPosDist()->SetCentreCoords(G4ThreeVector(0.0*cm, 0.0*cm, 0.0*cm));
  gps->GetCurrentSource()->GetAngDist()->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  
  gun = gps;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{ 
  gun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete gun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
