// $Id$
/**
 * @file
 * @brief implements user class SteppingAction
 */

#include "SteppingAction.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VTouchable.hh"
#include "G4ParticleDefinition.hh"
#include "G4MuonMinus.hh"
#include "G4MuonPlus.hh"
#include "G4UnitsTable.hh"
#include "Analysis.hh"

SteppingAction::SteppingAction()
{}

SteppingAction::~SteppingAction()
{}

void SteppingAction::UserSteppingAction(const G4Step* theStep)
{
    // --- Touchable / volume info
    const G4VTouchable* touchable =
        theStep->GetPreStepPoint()->GetTouchable();
    G4int volCopyNum = touchable->GetVolume()->GetCopyNo();
    G4String volName = touchable->GetVolume()->GetName();

    // =========================================================
    // 1) Energia depositata nel calorimetro EM (come prima)
    // =========================================================
    if (volCopyNum > 9 && volCopyNum < 100)   // EM calo step
    {
        Analysis::GetInstance()->AddEDepEM(theStep->GetTotalEnergyDeposit());
    }

    // =========================================================
    // 2) Analisi della step length
    // =========================================================
    const G4Track* track = theStep->GetTrack();
    const G4ParticleDefinition* particle = track->GetParticleDefinition();

    // Selezione particella: muoni
    if ( particle == G4MuonMinus::Definition() || particle == G4MuonPlus::Definition() )
    {
        // Selezione volume (esempio: aria)
        if (volName == "Air")
        {
            G4double stepLength = theStep->GetStepLength();

            if (stepLength > 0.)
            {
                Analysis::GetInstance()->AddStepLength(stepLength);
            }
        }
    }
}