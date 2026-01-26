#include "EventAction.hh"
//#include "RootSaver.hh"
#include "LaBr3Hit.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4Event.hh"
#include "SensitiveDetector.hh"

EventAction::EventAction() : 
    hene(0),
    rootSaver(0),
    histCollectionName("LaBr3HitCollection"),
    histCollID(-1) {}

EventAction::~EventAction() {};

void EventAction::AssignHisto(TH1F* histo) {
    hene = histo;
}

SensitiveDetector* EventAction::GetSensitiveDetector(const G4String& detname) {
    G4SDManager* SDMan = G4SDManager::GetSDMpointer();
    SensitiveDetector* sens = static_cast<SensitiveDetector*>(SDMan->FindSensitiveDetector(detname));
    return sens;
}

void EventAction::BeginOfEventAction(const G4Event* anEvent) {
    if (anEvent->GetEventID() % 1000 == 0) {
        G4cout << "Starting Event " << anEvent->GetEventID() << G4endl; 
    }

    if (histCollID == -1) {
        G4SDManager* SDMan = G4SDManager::GetSDMpointer();
        histCollID = SDMan->GetCollectionID(histCollectionName);
    }
}

void EventAction::EndOfEventAction(const G4Event* anEvent) {
    G4HCofThisEvent* hitsCollections = anEvent->GetHCofThisEvent();
    LaBr3HitCollection* hits = nullptr;

    if (hitsCollections) {
        hits = static_cast<LaBr3HitCollection*>(hitsCollections->GetHC(histCollID));
    }

    if (hits) {
        typedef std::vector<LaBr3Hit*>::const_iterator hitsVector_it;
        hitsVector_it hit = hits->GetVector()->begin();
        hitsVector_it end = hits->GetVector()->end();
        for (; hit != end; ++hit) {
            layer = (*hit)->GetLayerNumber();
            edep = (*hit)->GetEdep();
        }

        if (hene) hene->Fill(edep/keV);
    }

    G4String sdname = "/mydet/LaBr3";
    SensitiveDetector* sensitive = this->GetSensitiveDetector(sdname);
    //trackMap_t trackMap = sensitive->GetTrackMap();
    //if (rootSaver) rootSaver->AddEvent(trackMap, anEvent->GetEventID());
}
