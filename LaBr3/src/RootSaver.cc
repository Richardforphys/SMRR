#include "RootSaver.hh"
#include "LaBr3Hit.hh"
#include "TMath.hh"
#include <sstream>
#include <iostream>
#include <cassert>

RootSaver::RootSaver():
    rootFile(0),
    rootTree(0),
    runCounter(0),
    Tot_Tracks(0) {
        Part_ID = new Int_t[10000];
        Part_Moth_ID = new Int_t[10000];
        Part_Type = new Int_t[10000];
        MothPartType = new Int_t[10000];
        Part_EnDep = new Float_t[10000];
        Part_zStart = new Float_t[10000];
        MothPart_zStart = new Float_t[10000];
        Part_tStart = new Float_t[10000];
        MothPart_tStart = new Float_t[10000];
    };

RootSaver::~RootSaver() {
    if(rootTree) CloseTree();
};

void RootSaver::CreateTree(const std::string& fileName, const std::string& treeName) {
    if(rootTree) {
        std::cerr << "TTree already created! Call CloseTree mathod first!" << std::endl;
    }

    std::ostringstream fn;
    fn << fileName << "_run" << runCounter++ << ".root";

    rootFile = TFile::Open(fn.str().data(), "recreate");
    if (rootFile==0 || rootFile->IsZombie()) {
        std::cerr << "Error owhile opening the file" << fn.str() << "TTree will not be saved!" << std::endl;
    }

    else {
        G4cout << "rootFile correctly open!\n Name of the file is " 
        << rootFile->GetName() << G4endl;
        rootTree = new TTree(treeName.data(), treeName.data());

        //Define tree branches
        rootTree->Branch("ntracks", &Tot_Trakcs, "ntrack/I2"); // ntrack/I2 is 2bytes int 
        rootTree->Branch("id", Part_ID, "id[ntracks]/I2");
        rootTree->Branch("mum", Part_Moth_ID, "mum[ntracks]/I2");
        rootTree->Branch("type", Part_Type, "type[ntracks]/I");
        rootTree->Branch("mtype", MothPartType, "mtype[ntracks]/I");
        rootTree->Branch("edep", Part_EnDep, "edep[ntracks]/I2");
    }
}

