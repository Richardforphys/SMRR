// $Id: RunAction.cc 94 2010-01-26 13:18:30Z adotti $
/**
 * @file
 * @brief implements user class RunAction
 */

#include "RunAction.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "Analysis.hh"

RunAction::RunAction()
{}

RunAction::~RunAction()
{}

void RunAction::BeginOfRunAction(const G4Run* aRun )
{
  G4cout<<"Starting Run: "<<aRun->GetRunID()<<G4endl;
  Analysis::GetInstance()->PrepareNewRun(aRun);
}

void RunAction::EndOfRunAction( const G4Run* aRun )
{
  Analysis::GetInstance()->EndOfRun(aRun);
}
