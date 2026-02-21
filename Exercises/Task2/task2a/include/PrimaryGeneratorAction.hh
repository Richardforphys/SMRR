// $Id: PrimaryGeneratorAction.hh 283 2010-02-14 23:31:12Z selles $

#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

/**
 * @file
 * @brief Defines mandatory user class PrimaryGeneratorAction.
 */

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include <fstream>


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
  ~PrimaryGeneratorAction();
  //! defines primary particles (mandatory)
  void GeneratePrimaries(G4Event*);
private:
  G4ParticleGun* gun;
  std::ofstream * outfile;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
