#ifndef RUNACTION_HH
#define RUNACTION_HH

#include "G4UserRunAction.hh"
#include "TFile.h"
#include "TH1F.h"

class G4Run;
class EventAction;

class RunAction : public G4UserRunAction {
public:
    RunAction(EventAction* evAct);
    ~RunAction() override {}

    void BeginOfRunAction(const G4Run*) override ;
    void EndOfRunAction(const G4Run*) override ;

    TH1F* hrun{nullptr};
    TFile* out_root_f{nullptr};

private:
    EventAction* eventAction{nullptr};
};

#endif