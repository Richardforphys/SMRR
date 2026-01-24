#include "G4RunManager.hh"
#include "G4UIManager.hh"
#include "G4Version.hh"
#include "G4VisExecutive.hh" // Classe per Visual interface

//-----------------------------------------------------------Controllo versione G4-----------------------------------------------------
#if G4Version_NUMBER>=930
#include "G4UIExecutive.hh"
#else 
#include "G4UITerminal.hh"
#include "G4UItcsh.hh"
#endif

// Queste classi sono ancorra da definire! Sono include fondamentali, senza queste non va niente!
#include "DetectorConstruction.hh" //Geometry
#include "PraimaryGeneratorAction.hh" //Primary particles
#include "QGSP_BERT_HP.hh" // E'una physics list che G4 mette a disposizione

#include "EventAction.hh" //Non necessario per la simulazione ma per la compresione di quest'ultima
#include "RunAction.hh"
//------------------------------------------------------------------------------------------------------------------------------------------

int main(int argc, char** argv) {
    //Choose the random generator engine
    //This is the same as TRandom3 ROOT generator
    //It has a period of 2^19937-1
    CLHEP::HepRandom::setTheEngine(new CLHEP::MTwistEngine());

    //Run Manager
    G4RunManager* runManager = new G4RunManager();


    //Mandatory initialization classes (static during simulation) and action class (dynamic stuff during simulation)
    //1. 
    G4cout << "Setting Detector Geometry" << G4endl;
    G4VUserDetectorConstruction* detector = new DetectorConstruction(); 
    // Classi "V"irtuali: una sorta di indice che Geant mette a disposizione ma i cui membri e metodi non sono completamente definiti, se non nel nome
    runManager->SetUserIitialization(detector); 

    //2.
    G4cout << "Setting Physics List" << G4endl;
    G4VUserPhysicsList* physics = new QGSP_BERT_HP();
    runManager->SetUserIitialization(physics);

    //3.
    G4VUSerPrimaryGeneratorAction* gen_action = new PrimaryGeneratorAction();
    runManager->SetUserAction(gen_action);

    //Optional User Action Classes
    EventAction* event_acttion = new EventAction();
    RunAction* run_action = new RunAction(event_action);
    runManager->SetUserAction(event_action);
    runManager->SetUserAction(run_action);

    //Initialize the G4 Kernel
    runManager->initialize();

    //Visualization and UI commands
    G4VisManager* vis_manager = new G4VisExecutive();
    vis_manager->Initialize();

    //Get pointer to User Interface manager
    G4UIManager* UImanager = G4UIManager::GetUIPointer(); //E' un singleton --> posso creare un solo UI manager per main/simulazione. (if all'interno del costruttore)

    if(argc!=1):
        {
            //Se il numero di argomenti passati a command line è diverso da 1 (che sarebbe il nome del programma), sto entrando in batch mode
            G4String command = "/control/execute "; //Sto lanciando i comandi rinunciando alla visualizzazione, lo spazio dopo execute è importante
            G4String filename = argv[1]; //./exampleLaBr3 run.mac --> run.mac è proprio argv[1]
            UImanager->ApplyCommand(command+filename);
        }

    else {
        //interaactive mode: define UI session
        #if G4VERSION>=930
            G4UIExecutive* ui = new G4UIExecutive(argc, argv);
            if (ui->isGUI()): UImanager->ApplyCommand("/control/execute/visQT.mac");
            else UImanager->ApplyCommand("/control/execute/vis.mac");
        #else 
            #ifdef G4UI_USE_TCSH
                G4UISession* ui = new G4UITerminal(new G4UItcsh);
            #else
                G4UISession* ui = new G4UITerminal();
            #endif 
                UImanager->ApplyCommand("/control/execute vis.mac");
        #endif
            ui->SessionStart();
            delete ui;
    }

    delete runManager;
    return 0;
}