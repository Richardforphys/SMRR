#include "TrackInformation.hh"
#include "G4ios.hh"

G4Allocator<TrackInformation> aTrackInformationAllocator;

TrackInformation::TrackInformation()
{
    originalTrackID = 0;
    particleDefinition = 0;
    originalPosition = G4ThreeVector(0.,0.,0.);
    originalMomentum = G4ThreeVector(0.,0.,0.);
    originalEnergy = 0.;
    originalTime = 0.;
}

TrackInformation::TrackInformation(const G4Track* aTrack)
{
    originalTrackID = aTrack->GetTrackID();
    particleDefinition = aTrack->GetDefinition();
    originalPosition = aTrack->GetPosition();
    originalZPosition = ( aTrack->GetVertexPosition() ).getZ();
    originalMomentum = aTrack->GetMomentum();
    originalEnergy = aTrack->GetTotalEnergy();
    originalTime = aTrack->GetGlobalTime();
    originalTrackName = aTrack->GetDefinition()->GetParticleName();
    originalPDGEncoding = aTrack->GetDefinition()->GetPDGEncoding();
}

TrackInformation::TrackInformation(const TrackInformation* aTrackInfo)
{
    originalTrackID = aTrackInfo->originalTrackID;
    particleDefinition = aTrackInfo->particleDefinition;
    originalPosition = aTrackInfo->originalPosition;
    originalZPosition = aTrackInfo->originalZPosition;
    originalMomentum = aTrackInfo->originalMomentum;
    originalEnergy = aTrackInfo->originalEnergy;
    originalTime = aTrackInfo->originalTime;
    originalTrackName = aTrackInfo->originalTrackName;
    originalPDGEncoding = aTrackInfo->originalPDGEncoding;
}

TrackInformation::~TrackInformation(){;}

void TrackInformation::Print() const
{
    G4cout 
     << "Original track ID " << originalTrackID 
     << " at " << originalPosition << G4endl;
}
