#include "G4RunManager.hh"
#include "G4UImanager.hh"

#include "G4Version.hh"

#include "G4VisExecutive.hh"
#if  G4VERSION_NUMBER>=930
#include "G4UIExecutive.hh"
#else
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"
#endif

#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "PhysicsList.hh"
#include "EventAction.hh"
#include "RunAction.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/*!
\file
\brief Main program
*/


/*!
\brief Main program
\callgraph

*/
int main(int argc,char** argv)
{
  // Run manager
  G4RunManager * runManager = new G4RunManager;

  // mandatory Initialization classes 
  G4VUserDetectorConstruction* detector = new DetectorConstruction;
  runManager->SetUserInitialization(detector);
  G4cout << "Detector construction initialized" << G4endl;

  G4VUserPhysicsList* physics = new PhysicsList;
  runManager->SetUserInitialization(physics);
  G4cout << "Physics list initialized" << G4endl;
   
  // mandatory User Action classes
  G4VUserPrimaryGeneratorAction* gen_action = new PrimaryGeneratorAction;
  runManager->SetUserAction(gen_action);
  G4cout << "Primary generator action initialized" << G4endl;
  
  //Event action (handles for beginning / end of event)
  EventAction* anEventAction = new EventAction();
  runManager->SetUserAction( anEventAction );

  //Run action (handles for beginning / end of event)
  RunAction* aRunAction = new RunAction(anEventAction);
  runManager->SetUserAction( aRunAction );

  // Initialize G4 kernel
  runManager->Initialize();
  G4cout << "G4 kernel initialized" << G4endl;

  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
  G4cout << "Visualization manager initialized" << G4endl;
     
  // Get the pointer to the User Interface manager
  //
  G4UImanager * UImanager = G4UImanager::GetUIpointer();  

  if (argc!=1) {  // batch mode  
    
      G4String command = "/control/execute ";
      G4String fileName = argv[1];
      UImanager->ApplyCommand(command+fileName);
  }
  else {           // interactive mode : define UI session
     
#if  G4VERSION_NUMBER>=930
    G4UIExecutive * ui = new G4UIExecutive(argc,argv);
    if (ui->IsGUI()) 
      UImanager->ApplyCommand("/control/execute macros/visQt.mac");
    else 
      UImanager->ApplyCommand("/control/execute macros/vis.mac");     
#else
  #ifdef G4UI_USE_TCSH
    G4UIsession * ui = new G4UIterminal(new G4UItcsh);      
  #else
    G4UIsession * ui = new G4UIterminal();
  #endif
    UImanager->ApplyCommand("/control/execute macros/vis.mac");     
#endif
    ui->SessionStart();
    delete ui;     
  }

  // Free the store: user actions, physics_list and detector_description are
  //                 owned and deleted by the run manager, so they should not
  //                 be deleted in the main() program !

  delete runManager;

  return 0;
}
