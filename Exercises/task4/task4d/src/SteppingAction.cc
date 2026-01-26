#include "SteppingAction.hh"
//#include "G4Track.hh"
#include "G4Step.hh"
//#include "G4ParticleDefinition.hh"
//#include "G4ParticleTypes.hh"
//#include "G4StepPoint.hh"
//#include "G4VPhysicalVolume.hh"
#include "G4VTouchable.hh"
//#include "G4TouchableHistory.hh"
#include "G4SteppingManager.hh"
#include "G4UnitsTable.hh"
#include "Analysis.hh"

SteppingAction::SteppingAction()
{
}

SteppingAction::~SteppingAction()
{
}

void SteppingAction::UserSteppingAction( const G4Step * theStep ) {
	//Check if this is the first step of this event,
	//in this case print out info and prepare for next event
	//Ask the stepping manager if this is the first step of this
	//event
	//G4cout<<fpSteppingManager->GetFirstStep()<<G4endl;
	//if ( fpSteppingManager->GetFirstStep() )
	//{
	//	G4cout<<"Previous Event had : "<<G4BestUnit(totalEdepEM,"Energy")<<G4endl;
	//	PrepareNewEvent();
	//}
	//We need to know if this step is done inside the EM calo or not.
	//We ask the PreStepPoint the volume copy number.
	//Remember: EM calo has copy num >9 and <100
	//We could have asked the volume name, but string
	//comparison is not efficient
	const G4VTouchable* touchable = theStep->GetPreStepPoint()->GetTouchable();
	G4int volCopyNum = touchable->GetVolume()->GetCopyNo();
	if ( volCopyNum > 9 && volCopyNum  < 100 ) //EM calo step
	{
		Analysis::GetInstance()->AddEDepEM( theStep->GetTotalEnergyDeposit() );
	}
}

