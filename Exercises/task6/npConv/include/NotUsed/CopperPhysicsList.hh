// This code implementation is the intellectual property of
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// Class Description:
// The list of particles and processes are defined in this class.
// Class Description - end
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef CopperPhysicsList_h
#define CopperPhysicsList_h 1

#include "G4VUserPhysicsList.hh"
#include "globals.hh"
#include "G4VProcess.hh"
#include "G4LowEnergyIonisation.hh"
#include "G4LowEnergyBremsstrahlung.hh"
#include "G4eIonisation.hh"
#include "G4VMultipleScattering.hh"

//class CopperDetectorConstruction;
//class CopperPhysicsListMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class CopperPhysicsList: public G4VUserPhysicsList
{
public: // Without description

    CopperPhysicsList();
   ~CopperPhysicsList();

  protected:
    // Construct particle and physics
    void ConstructParticle();
    void ConstructProcess();
 
    void SetCuts();

  protected:
    // these methods Construct particles 
    void ConstructBosons();
    void ConstructLeptons();
    void ConstructMesons();
    void ConstructBarions();
    void ConstructIons();

  protected:
  // these methods Construct physics processes and register them
    void ConstructGeneral();
    void ConstructEM();
    void ConstructHad();
    
  public: // Without description

    void SetGammaCut(G4double);
    void SetElectronCut(G4double);
    void SetProtonCut(G4double);
    void SetAlphaCut(G4double);
    void SetIonCut(G4double);

  private:

    G4double cutForGamma;
    G4double cutForElectron;
    G4double cutForProton;
    G4double cutForAlpha;
    G4double cutForIon;
    G4double cutForTriton;
    G4VProcess * thePhotoElectricEffect;
    G4VProcess * theComptonScattering;
    G4VProcess * theGammaConversion;
    G4VProcess * theGammaRayleigh;
    G4VProcess * theeminusMultipleScattering;
    G4LowEnergyIonisation * theeminusLEIonisation;
    G4LowEnergyBremsstrahlung * theeminusLEBremsstrahlung;
    G4eIonisation * theeminusIonisation;
    G4VProcess * theeminusBremsstrahlung;
    G4VProcess * theeplusMultipleScattering;
    G4eIonisation * theeplusIonisation;
    G4VProcess * theeplusBremsstrahlung;
    G4VProcess * theeplusAnnihilation;

//    CopperDetectorConstruction* pDet;
  //CopperPhysicsListMessenger* physicsListMessenger;
};

#endif



