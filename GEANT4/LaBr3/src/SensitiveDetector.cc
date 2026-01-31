#include "SensitiveDetector.hh"
#include "G4Step.hh"
#include "Randomize.hh"
#include "G4HCofThisEvent.hh"
#include "G4HCtable.hh"
#include "G4UnitsTable.hh"
#include "G4TouchableHistory.hh"

#include "TrackParticle.hh"
#include "TrackParentParticle.hh"

SensitiveDetector::SensitiveDetector(G4String SDname)
    :G4VSensitiveDetector(SDname) {

    G4cout << "Creating Sensitive Detector: " << SDname << G4endl; 
    
    //"cllectionName" is  protected member of base class G4VSensitiveDetector
    // here we add the name of the hit collection
    G4String myCollectionName = "LaBr3HitCollection";

    //Add myCollectionName to the collectionName vector
    collectionName.insert(myCollectionName);
}

SensitiveDetector::~SensitiveDetector() {
    //G4cout << "Destroying Sensitive Detector" << G4endl;
}

G4bool SensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory* R0Hist) {
    //This method is called at each step in the Sensitive Detector

    G4cout << "Processing hits..." << G4endl;

    G4TouchableHandle touchable = step->GetPreStepPoint()->GetTouchableHandle();
    G4int copyNo = touchable->GetVolume(0)->GetCopyNo(); //Get the copy number of the current volume
    G4int LaBr3index = copyNo;

    TrackParticle* TParticle = new TrackParticle();
    G4Track* thistrack = step->GetTrack();
    G4double trackenergy = thistrack->GetDynamicParticle()->GetKineticEnergy();
    G4int thistrackID = thistrack->GetTrackID();//Define the key for trackMap_t
    G4String trackname = thistrack->GetDefinition()->GetParticleName();
    G4int parentID = thistrack->GetParentID();

    G4cout << " Track ID: " << thistrackID << " Particle: " << trackname 
           << " Energy: " << G4BestUnit(trackenergy, "Energy") << G4endl;
    
    G4int trackPDGencod = thistrack->GetDefinition()->GetPDGEncoding();
    G4double zstartpos = (thistrack->GetVertexPosition()).getZ();
    G4double starttime = thistrack->GetGlobalTime();

    //Filling the member of TrackParticle
    TParticle->SetMothPart_ID(parentID);
    //...user will define...

    //Hits
    hitMap_t::iterator it = HitMap.find(LaBr3index); //Look for existing hit for this LaBr3 crystal
    LaBr3Hit* aHit = nullptr;

        if(it != HitMap.end()) { //Hit already exists for this LaBr3 crystal
            aHit = it->second;
        } else { //Create new hit
            aHit = new LaBr3Hit(LaBr3index);
            HitMap.insert(std::make_pair(LaBr3index, aHit));
            LaBr3HitCollection->insert(aHit);
        }
    aHit->AddEdep(edep); // This method is defined in LaBr3Hit.hh, to be written
    return true;
}

void SensitiveDetector::Initialize(G4HCofThisEvent* HCE) {
    //Creation of hte hit collection
    // collectionName[0] is "LaBr3HitCollection", as declared in the 
    // class constructor
    hitCollection = new LaBr3HitCollection(GetName(), collectionName[0]);
    static G4int HCID = -1;
    if (HCID<0) HCID = GetCollectionID(0); //get an ID for collectionName[0]
    HCE->AddHitsCollection(HCID, hitCollection);
    HitMap.clear();
    trackMap.clear();
}

void SensitiveDetector::EndOfEvent(G4HCofThisEvent* HCE){
    hitCollection->PrintAllHits(); //?
}