#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

/*!
\file
\brief defines mandatory user class PrimaryGeneratorAction
*/

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4GeneralParticleSource.hh"


class G4VPrimaryGenerator;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
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
  //  G4VPrimaryGenerator* gun;
  G4GeneralParticleSource* gun;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
