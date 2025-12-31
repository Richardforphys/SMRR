/*This is the main file for the LaBr3 simulation project*/

//-------------------------------------------- INCLUDES ----------------------------------------------------
#include "G4RunManager.hh" // run manager
#include "G4UIManger.hh" //manager user interface
#include "G4Version.hh" //compatibilità tra versioni di geat4
#include "G4VisExecutive.hh" //per far andare l'itnerfaccia per la visualizzazione

#if G4VERSION_NUMBER>=930
#include "G4UIExecutive.hh"
#else
#include "G4UITerminal.hh"
#include "G4UItcsh.hh" //tcsh è un tipo di shell
#endif

#include "DetectorConstruction.hh" // includo lib per geometry
#include "PrimayGEneratorAction.hh" // includo lib per interazione
#include "QGSP_BERT_HP.hh"  // QUAK GLUO SYMMETRY...

#include "EventAction.hh"
#include "RunAction.hh"