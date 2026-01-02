/*This is the main file for the LaBr3 simulation project*/

/*================================================Includes================================================*/
#include "G4RunManager.hh" // run manager
#include "G4UImanager.hh" //manager user interface
#include "G4Version.hh" //compatibilità tra versioni di geat4
#include "G4VisExecutive.hh" //per far andare l'itnerfaccia per la visualizzazione

#if G4VERSION_NUMBER>=930
#include "G4UIExecutive.hh"
#else
#include "G4UITerminal.hh"
#include "G4UItcsh.hh" //tcsh è un tipo di shell
#endif
#include "G4VUserPhysicsList.hh"
#include "DetectorConstruction.hh" // includo lib per geometry
#include "PrimaryGeneratorAction.hh" // includo lib per interazione
#include "QGSP_BERT_HP.hh"  // QUARK GLUO SYMMETRY...

#include "EventAction.hh"
#include "RunAction.hh"

/*================================================MAIN================================================*/

int main (int argc, char** argv){
    //Choose the andom GEnerator Engine
    //This is the same as TRandom3 ROOT generator, it has a period of 2^19937-1
    CLHEP::HepRandom::setTheEngine(new CLHEP::MTwistEngine()); //   

    G4RunManager* runManager = new G4RunManager();   

    /*====================Mandatory Initialization Classes====================*/
    G4VUserDetectorConstruction* detector = new DetectorConstruction();
    //V sta per virtual...uno chassis di una classe che Geant mette a disposizione, i cui metodi sono defiti ma non implementati
    //Dunque dovremo crere una classe chiamata DetectorConstruction all'interno della quale definiamo tutto quello che riguarda il nostro detector
    runManager->SetUserInitialization(detector);

    G4cout << "Setting Physics List" << G4endl;
    G4VUserPhysicsList* physics = new QGSP_BERT_HP(); //stesso discorso per DetectorConstruction
    runManager->SetUserInitialization(physics);

    /*====================Mandatory Action Classes====================*/
    G4VUserPrimaryGeneratorAction* gen_action = new PrimaryGeneratorAction();
    runManager->SetUserAction(gen_action);

    /*====================Optional USer Action Classes====================*/
    EventAction* event_action = new EventAction();
    RunAction* run_action = new RunAction(event_action);
    runManager->SetUserAction(event_action);
    runManager->SetUserAction(run_action);

    /*====================Initialize the G4 Kernel====================*/
    runManager->Initialize();

    /*====================Visualization and UI====================*/

    G4VisManager* visManager = new G4VisExecutive();
    visManager->Initialize();

    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    if(argc!=1)    {
        //Batch mode --> faccio partire la simulazione senza visualizzazione nè altro...solo risultati numerici
        G4String command = "/control/execute";
        G4String filename = argv[1];
        UImanager->ApplyCommand(command+filename);        
    }
    else {
        #if G4VERSION_NUMBER>=930
            //Interactive Mode: define UI session
            G4UIExecutive* ui = new G4UIExecutive(argc, argv);
            if (ui->IsGUI()) UImanager->ApplyCommand("control/execute/visQT.mac"); // se la UI è una GuidedUI, devo utilizzare il programma qt
            else UImanager->ApplyCommand("/control/execute vis.mac");
        #else
            #ifdef G4UI_USE_TCSH //è una variabile d'ambiente. Se è definita, usa shell tcsh per eseguire comando, altrimenti usa default
                G4UIsession* ui = new G4UITerminal(new G4UItcsh);
            #else 
                G4UIsession* ui = new G4UITerminal();
            #endif 
                UImanager->ApplyCommand("/control/execute vis.mac")
        #endif
            ui->SessionStart();
            delete ui;
    }

    delete runManager;

    return 0;
}