#include "NeutronGEMTrackInformation.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"

G4ThreadLocal G4Allocator<NeutronGEMTrackInformation> * aTrackInformationAllocator = 0;
//G4Allocator<NeutronGEMTrackInformation> aTrackInformationAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
NeutronGEMTrackInformation::NeutronGEMTrackInformation()
  : G4VUserTrackInformation()
{
    fOriginalTrackID = 0;
    fParticleDefinition = 0;
    fOriginalPosition = G4ThreeVector(0.,0.,0.);
    fOriginalMomentum = G4ThreeVector(0.,0.,0.);
    fOriginalEnergy = 0.;
    fOriginalTime = 0.;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
NeutronGEMTrackInformation::NeutronGEMTrackInformation(const G4Track* aTrack)
  : G4VUserTrackInformation()
{
    fOriginalTrackID = aTrack->GetTrackID();
    fParticleDefinition = aTrack->GetDefinition();
    fOriginalPosition = aTrack->GetPosition();
    fOriginalMomentum = aTrack->GetMomentum();
    fOriginalEnergy = aTrack->GetKineticEnergy();
    fOriginalTime = aTrack->GetGlobalTime();
  }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
NeutronGEMTrackInformation
::NeutronGEMTrackInformation(const NeutronGEMTrackInformation* aTrackInfo)
  : G4VUserTrackInformation()
{
    fOriginalTrackID = aTrackInfo->fOriginalTrackID;
    fParticleDefinition = aTrackInfo->fParticleDefinition;
    fOriginalPosition = aTrackInfo->fOriginalPosition;
    fOriginalMomentum = aTrackInfo->fOriginalMomentum;
    fOriginalEnergy = aTrackInfo->fOriginalEnergy;
    fOriginalTime = aTrackInfo->fOriginalTime;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
NeutronGEMTrackInformation::~NeutronGEMTrackInformation()
{;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
NeutronGEMTrackInformation& NeutronGEMTrackInformation
::operator =(const NeutronGEMTrackInformation& aTrackInfo)
{
    fOriginalTrackID = aTrackInfo.fOriginalTrackID;
    fParticleDefinition = aTrackInfo.fParticleDefinition;
    fOriginalPosition = aTrackInfo.fOriginalPosition;
    fOriginalMomentum = aTrackInfo.fOriginalMomentum;
    fOriginalEnergy = aTrackInfo.fOriginalEnergy;
    fOriginalTime = aTrackInfo.fOriginalTime;


    return *this;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void NeutronGEMTrackInformation::Print() const
{

    G4cout
      << "Original primary track ID " << fOriginalTrackID << " ("
      << fParticleDefinition->GetParticleName() << ","
     << fOriginalEnergy/keV << "[keV])\n" << G4endl;
}

