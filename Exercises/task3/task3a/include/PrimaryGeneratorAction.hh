// $Id: PrimaryGeneratorAction.hh 94 2010-01-26 13:18:30Z adotti $
/**
 * @file
 * @brief Defines mandatory user class PrimaryGeneratorAction.
 */


#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"

class G4VPrimaryGenerator;
 
/*!
\brief This mandatory user class provides the primary particle generator

Geant4 provides a number of predefined primary particle generator, to be utilised by the user.
 - G4ParticleGun
 - G4GeneralParticleSource

\sa GeneratePrimaries()
 */
class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  //! constructor
  PrimaryGeneratorAction();
  //! destructor
  virtual ~PrimaryGeneratorAction();
  //! defines primary particles (mandatory)
  virtual void GeneratePrimaries(G4Event*);
private:  

  G4VPrimaryGenerator* InitializeGPS();

  G4VPrimaryGenerator* gun;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
