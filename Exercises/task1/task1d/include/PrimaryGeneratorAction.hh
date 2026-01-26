//
// Derived from ExN02PrimaryGeneratorAction.hh
//
 
#ifndef  PrimaryGeneratorAction_h
#define  PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"

class DetectorConstruction;
class G4ParticleGun;
class G4Event;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
class  PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction() ;  //  (DetectorConstruction*);    
   ~PrimaryGeneratorAction();

  public:
    void GeneratePrimaries(G4Event*);

  private:
    G4ParticleGun* particleGun;
    // DetectorConstruction* myDetector;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


