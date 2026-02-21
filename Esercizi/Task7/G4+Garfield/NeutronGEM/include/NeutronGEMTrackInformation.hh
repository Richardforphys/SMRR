#ifndef NeutronGEMTrackInformation_h
#define NeutronGEMTrackInformation_h 1

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleDefinition.hh"
#include "G4Track.hh"
#include "G4Allocator.hh"
#include "G4VUserTrackInformation.hh"

class NeutronGEMTrackInformation : public G4VUserTrackInformation
{
public:
  NeutronGEMTrackInformation();
  NeutronGEMTrackInformation(const G4Track* aTrack);
  NeutronGEMTrackInformation(const NeutronGEMTrackInformation* aTrackInfo);
  virtual ~NeutronGEMTrackInformation();

  inline void *operator new(size_t);
  inline void operator delete(void *aTrackInfo);

  NeutronGEMTrackInformation& operator =(const NeutronGEMTrackInformation& right);

  virtual void Print() const;

  G4ParticleDefinition* GetParticleDefinition() {return fParticleDefinition;}
  G4double GetKineticEnergy() {return fOriginalEnergy; }


private:
  // Information of the primary track at the primary vertex
  G4int                 fOriginalTrackID;  // Track ID of primary particle
  G4ParticleDefinition* fParticleDefinition;
  G4ThreeVector         fOriginalPosition;
  G4ThreeVector         fOriginalMomentum;
  G4double              fOriginalEnergy;
  G4double              fOriginalTime;



};

extern G4ThreadLocal G4Allocator<NeutronGEMTrackInformation> * aTrackInformationAllocator;
//extern  G4Allocator<NeutronGEMTrackInformation> aTrackInformationAllocator;

inline void* NeutronGEMTrackInformation::operator new(size_t)
{
  if(!aTrackInformationAllocator)
    aTrackInformationAllocator = new G4Allocator<NeutronGEMTrackInformation>;
  return (void*)aTrackInformationAllocator->MallocSingle();
}

inline void NeutronGEMTrackInformation::operator delete(void *aTrackInfo)
{ aTrackInformationAllocator->FreeSingle((NeutronGEMTrackInformation*)aTrackInfo);}

#endif

