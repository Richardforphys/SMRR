#include "StackingAction.hh"
#include "G4ClassificationOfNewTrack.hh"
#include "G4Track.hh"

StackingAction::StackingAction():
    old_TrackID(-1) {
}

StackingAction::~StackingAction() {}

G4ClassificationOfNewTrack StackingAction::ClassifyNewTrack(const G4Track* aTrack) {

    //At the beginning all tracks are Urgent
    G4ClassificationOfNewTrack result (fUrgent);

    //I want to get the number ID of the previous track
    old_TrackID = aTrack->GetParentID();

    //If the previouse track was generated before the current track, the present one is 
    //put on waiting state untill all the Urgent tracks will be processed

    if(aTrack->GetTrackID() > old_TrackID) { //It means this is a secondary track
        result = fWaiting;
    } else { 
        result = fUrgent;
    }

    return result;
}