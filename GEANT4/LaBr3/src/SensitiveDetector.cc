#include "SensitiveDetector.hh"
#include "G4Step.hh"
#include "G4HCofThisEvent.hh"
//#include "G4HCTable.hh"
#include "G4SDManager.hh"
#include "G4TouchableHistory.hh"

SensitiveDetector::SensitiveDetector(G4String SDname) : G4VSensitiveDetector(SDname) {
    G4cout<<"Creating SD with name " << SDname << G4endl;
    G4String myCollectionName = "LaBr3HitCollection";

    //Add myCollectionName to the vector of names available in the variable called CollectionName
    //CollectionName is a protected data member of the base class G4VSensitiveDetector

    collectionName.insert(myCollectionName);
}

SensitiveDetector::~SensitiveDetector(){}

G4bool SensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory *) {
    //This method is called everytime a G4Step is performed in the 
    //logical volume to which the SD is attached (LabBr3 detector)
    G4Touchable touchebale = step->GetPreStepPoint()->GetTouchableHandle();
    G4int copyNo = touchable->GetVolume(0)->GetCopyNo();
    G4int LaBr3Index = copyNo;

    /*----------------Customization-------------------*/
    TrackParticle* TParticle = new TrackParticle();
    G4Track* thistrack = step->GetTrack();

    G4double trackenergy = thistrack->GetDynamicParticle()->GetKineticEnergy(); //step by step kinetic energy
    //Define the key for the trackMap_t
    G4int trackId = thistrack->GetTrackId();
    G4String trackName = thistrack->GetDefinition()->GetParticleName();
    G4int parentID = thistrack->GetParentID();
    G4int trackPDGencod = thistrack->GetDefinition->GetPDGEncoding();
    G4double zstartpos = (thistrack->GetVertexPosition()).getZ();
    G4double startime =thistrack->GetGlobalTime();
    G4double edep = step->GetTotalEnergyDeposit();

    //Give all retrieved infos to custom-made classes

    Tparticle->SetMothPart_ID(parent_ID);
    Tparticle->SetPart_Type(trackPDGencod);
    Tparticle->SetPart_name(trackName);
    Tparticle->SetZStart_Pos(zstartpos);
    Tparticle->SetStart_Time();

    if (edep/MeV > 20) {
        G4cout << "ISSUE in EDEP: deposited energy is " << G4BestUnit(edep, "Energy") << " from particle " << trackname 
        << " ID " << thistrackID << " from particle " << trackname << " ID " << thistrackID
        << " with mum " << parent_ID << " produced at " << G4BestUnits(zstartpos, "Lenght") << G4endl;
    }

    /*----------------Customization End-------------------*/

    hitMap_t::iterator it = hitMap.find(LaBr3Index) //it is the iterator index
    LaBr3Hit* aHit = 0;

    if (it != hitMap.end()) {
        //Hit for this volume already exists and we must remember the hit pointer
        aHit = it->second;
    }
    else {
        //Hits for this volume do not exist-->create one
        aHit = new LaBr3Hit(LaBr3Index);
        hitMap.insert(std::make_pair(LaBr3Index, aHit));
        hitCollection->insert(aHit);
    }

    aHit->AddEdep(edep);
}