#include "G4RunManager.hh"
#include "G4UIManager.hh"
#include "G4Version.hh"

#include "G4VisExecutive.hh" // visual interface
#if G4VERSIONE_NUMBER >= 930
#include "G4UIExecutive.hh"
#else
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"
#endif

// Classi ancora da definire. Sono fondamentali per l'esecuzione del programma
#include "DetectorConstruction.hh" //Geometria
#include "PrimaryGeneratorAction.hh" //Primary particles
#include "QGSP_BERT_HP.hh" //Physics list

// Non necessari ma molto utili a capire quello che succede  
#include "EventAction.hh"
#include "RunAction.hh"

int main(int argc, chat** argv)
{

//Choose the Random Generator Engine
//This is the same as TRandom3 ROOT generator
//Period of 2^19937-1
CLHEP::HepRandom::setTheEngine(new CLHEP::MTwistEngine());

//Run Manager
G4RunManager* runManager = new G4RunManager();

//Mandatory Initialization Class (static elements)
G4VUserDetectorConstruction* detector = new DetectorConstruction();  //Virtual class for the detector construction
runManager -> SetUserInitialization(detector); 

G4cout << "Setting Physics List" << G4endl;

//Mandatory Initialization Class 
G4VUserPhysicsList* physics = new QGSP_BERT_HP(); //Physics list
runManager -> SetUserInitialization(physics);

//Mandatory Action Class  (dynamic elements)
G4VUserPrimaryGeneratorAction* gen_action = new PrimaryGeneratorAction(); //Primary particles
runManager -> SetUserAction(gen_action);

//Optional User Action Classes
EventAction* event_action = new EventAction();
RunAction* run_action = new RunAction(event_action);
runManager -> SetUserAction(event_action);
runManager -> SetUserAction(run_action);

//Initialize G4 kernel
runManager -> Initialize();

//Visualization
G4VisManager* visManager = new G4VisExecutive();
visManager -> Initialize(); 

//User Interface
G4UIManager* UImanager = G4UImanager::GetUIpointer();  //Get the pointer to the User Interface Manager

if (argc != 1) 
{
    // batch mode if the number of arguments is > 1
    G4String command = "/control/execute ";  //command without visualization
    G4String filename = argv[1];  //example: ./exampleLaBr3 run.mac _> run.mac is argv[1]
    UImanager -> ApplyCommand(command+filename);
}
else 
{
    // interactive mode : define UI session
#if G4VERSIONE_NUMBER >= 930
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);
    if(ui->isGUI()) UImanager->ApplyCommand("/control/execute visQT.mac");
    else UImanager->ApplyCommand("/control/execute vis.mac");
#else
  #ifdef G4UI_USE_TCSH   
    G4UIsession* ui = new G4UIterminal(new G4UItcsh); //tcsh terminal
  #else 
    G4UIsession* ui = new G4UIterminal(); //default terminal
  #endif
    UImanager -> ApplyCommand("/control/execute vis.mac");
#endif

    ui -> SessionStart();
    delete ui;
}

delete runManager;

return 0;
}