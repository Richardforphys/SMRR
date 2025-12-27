#include "G4RunManager.hh" //file .hh che serve da includere per usare G4RunManager
#include "G4UImanager.hh" //file .hh che serve da includere per usare G4UImanager

// Geant4 funziona con classi che eseguono e classi che gestiscono, ad esempio G4RunManager 
// esegue il run e G4UImanager gestisce l'interfaccia utente

#include "G4Version.hh"
#include "G4VisExecutive.hh"
#include "DetectorConstruction.hh" //classe non di geant4, che scriveremo noi per definire il nostro rivelatore
#include "PrimaryGeneratorAction.hh" //classe non di geant4, che scriveremo noi per definire la sorgente di particelle
//primarie che ci interessano
#include "QGSP_BIC_HP.hh" //classe che definisce la fisica da usare