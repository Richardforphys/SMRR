#include "RunAction.hh"
#include "EventAction.hh"
#include "G4Run.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

RunAction::RunAction(EventAction* theEventAction) 
    : eventAction(theEventAction)
{
    // If using RootSaver, initialize here
    // eventAction->SetRootSaver(&saver);
}

void RunAction::BeginOfRunAction(const G4Run* aRun) {
    G4cout << "Starting Run: " << aRun->GetRunID() << G4endl;
    G4cout << "Creating ROOT TTree and histogram..." << G4endl;

    // Uncomment if using RootSaver
    // saver.CreateTree();

    out_root_f = new TFile("histoEnergy.root", "RECREATE");
    hrun = new TH1F("EnDis", "Energy Distribution", 1000, 0., 1000.);
    hrun->GetXaxis()->SetTitle("Deposited Energy (keV)");
    if(eventAction) eventAction->AssignHisto(hrun);
}

void RunAction::EndOfRunAction(const G4Run* aRun){
    G4cout << "Ending Run: " << aRun->GetRunID() << G4endl;
    G4cout << "Number of Events: " << aRun->GetNumberOfEvent() << G4endl;

    if(hrun) hrun->Write();
    if(out_root_f) out_root_f->Close();
}
