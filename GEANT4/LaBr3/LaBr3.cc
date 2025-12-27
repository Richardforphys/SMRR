/*=============================================== INCLUDES ================================================================*/

#include "G4RunManager.hh" //file .hh che serve da includere per usare G4RunManager
#include "G4UImanager.hh" //file .hh che serve da includere per usare G4UImanager

/* Geant4 funziona con classi che eseguono e classi che gestiscono, ad esempio G4RunManager 
esegue il run e G4UImanager gestisce l'interfaccia utente*/

#include "G4Version.hh"
#include "G4VisExecutive.hh"
#include "DetectorConstruction.hh" //classe non di geant4, che scriveremo noi per definire il nostro rivelatore
#include "PrimaryGeneratorAction.hh" /*classe non di geant4, che scriveremo noi per definire la sorgente di particelle
primarie che ci interessano*/
#include "QGSP_BIC_HP.hh" //PhysicsList, classe che definisce la fisica da usare
#include "EventAction.hh" //classe non di geant4, che scriveremo noi per definire le azioni da eseguire ad ogni evento
#include "RunAction.hh" //classe non di geant4, che scriveremo noi
/*Quelle che finiscono con "action" sono tutte classi necessarie per la definizione della dinamica della simulazione*/
using namespace std;

/*=============================================== MAIN ================================================================*/

int main(int argc, char** argv) {
    //choose the random engine
    //using the same as TRandom3 of ROOT, it has a period of 2^19937-1
    CLHEP::HepRandom::setTheEngine(new CLHEP::MTwistEngine);

    // Construct the default run manager
    G4RunManager* runManager = new G4RunManager();
    
    // Set mandatory initialization classes
    G4VUserDetectorConstruction* detector = new DetectorConstruction(); //instantiating pointer to detector geometry
    runManager->SetUserInitialization(detector); 
    
    G4VUserPhysicsList* physics = new QGSP_BIC_HP(); 
    runManager->SetUserInitialization(physics);

    G4VUserPrimaryGeneratorAction* gen_action = new PrimaryGeneratorAction();
    runManager->SetUserAction(gen_action);

    //Optional user action classes
    EventAction* event_action = new EventAction();
    RunAction* run_action = new RunAction(event_action);
    runManager->SetUserAction(event_action);
    runManager->SetUserAction(run_action);
    
    // Initialize G4 kernel
    runManager->Initialize(); //Lanciando questa, abbiamo "chiuso" la geometria e la fisica

    // Initialize visualization
    G4VisManager* visManager = new G4VisExecutive();
    visManager->Initialize();

    //Normal execution vs batch mode (niente visualizzazione, carica direttamente geometria e particelle ed esegue simulazione)
    G4UImanager* UImanager = G4UImanager::GetUIpointer(); //prendo il puntatore all'interfaccia utente, occhio che è un singleton
    if (argc != 1) { //batch mode
        G4String command = "/control/execute "; //comando che esegue un macro file, GEant ha i suoi tipi! (non string, G4String)
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command + fileName);
    } 

    else { //interactive mode
        G4UIExecutive* ui = new G4UIExecutive(argc, argv);
        UImanager->ApplyCommand("/control/execute visQT.mac");
        ui->SessionStart();
        delete ui;
    }

    // Job termination
    delete visManager;
    delete runManager;
    return 0;
}