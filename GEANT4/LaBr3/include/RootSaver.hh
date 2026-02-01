#include <string>
#include "TTree.h"
#include "TFile.h"
#include "G4UnitsTable.hh"
#include "LaBr3Hit.hh"
#include "TrackParticle.hh"       //User defined class
#include "TrackParentParticle.hh" //User defined class
#include "EventAction.hh"

class RootSaver {
    public:
        RootSaver();
        ~RootSaver();

        typedef std::map<G4int, TrackParticle*> trackMap_t;
        
        void CreateTree(const std::string& fileName = "tree", const std::string& treeName="LaBr3_Tree");
        void CloseTree();
        void AddEVent(trackMap_t tMap, G4int evID);

    private:
        TFile* rootFile;
        TTree rootTree;
        unsigned int runCounter; //run counter to uniquely associate a TFile with a Run
        //TTree variables
        Int_t Tot_Tracks; //int_t is a ROOT native type 
        Int_t* Part_ID;
        Int_t* Part_Moth_ID;
        Int_t* PartType;
        Int_t* MothPartType;
        Float_t* Part_EnDep;
        Float_t* MothPart_EnDep;
        Float_t* Part_zStart;
        Float_t* MothPart_zStart;
        Float_t* Part_tStart;
        Float_t* MothPart_tStart;
        Int_t Event_ID;

        typedef std::map<G4int, TrackParentParticle> TrackParentMap_t;
        TrackParentMap_t GetParentProperties(trackMap_t TParticleMap);
}