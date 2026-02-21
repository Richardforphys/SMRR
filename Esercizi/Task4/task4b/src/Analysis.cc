/*
 * Analysis.cc
 *
 *  Created on: 9 Feb 2010
 *      Author: adotti
 */

#include "Analysis.hh"
#include "G4UnitsTable.hh"


Analysis* Analysis::singleton = 0;

Analysis::Analysis() :
	thisEventTotEM(0),
	thisEventSecondaries(0),
	thisRunTotEM(0),
	thisRunTotSecondaries(0),
	thisEventNumGammas(0),
	thisRunNumGammas(0),
	thisEventNumNeutrons(0),
	thisRunNumNeutrons(0)
{
}

void Analysis::PrepareNewEvent(const G4Event* /*anEvent*/)
{
	//Reset variables relative to this event
	thisEventSecondaries = 0;
	thisEventTotEM = 0;
	for ( int i = 0; i<NUMLAYERS ; ++i ) thisEventTotHad[i]=0;
	thisEventNumGammas = 0;
	thisEventNumNeutrons = 0;
}

void Analysis::PrepareNewRun(const G4Run* /*aRun*/ )
{
	//Reset variables relative to the run
	thisRunTotEM = 0;
	thisRunTotSecondaries = 0;
	for ( int i = 0; i<NUMLAYERS ; ++i ) thisRunTotHad[i]=0;
	thisRunNumGammas = 0;
	thisRunNumNeutrons =0;
}

void Analysis::EndOfEvent(const G4Event* /*anEvent*/)
{
	//Accumulate over the run
	thisRunTotEM += thisEventTotEM;
	thisRunTotSecondaries += thisEventSecondaries;
	for ( int i = 0; i<NUMLAYERS ; ++i ) thisRunTotHad[i] += thisEventTotHad[i];
	thisRunNumGammas += thisEventNumGammas;
	thisRunNumNeutrons += thisEventNumNeutrons;
	//Uncomment these line for more verbosity:
	//G4cout<<"Event: "<<anEvent->GetEventID() <<" Energy in EM calo: "<<G4BestUnit(thisEventTotEM,"Energy")<<" Secondaries: "<<thisEventSecondaries<<G4endl;
}

void Analysis::EndOfRun(const G4Run* aRun)
{
	//Some print outs
	G4int numEvents = aRun->GetNumberOfEvent();

	G4double totHadCalo = 0;
	for ( int i = 0 ; i < NUMLAYERS ; ++i) totHadCalo += thisRunTotHad[i];

	G4cout<<"================="<<G4endl;
	G4cout<<"Summary for run: "<<aRun->GetRunID()<<G4endl;
	G4cout<<"\t Event processed: "<<numEvents<<G4endl;
	G4cout<<"\t Average number of secondaries: "<<thisRunTotSecondaries/numEvents<<G4endl;
	G4cout<<"\t Average energy in EM calo: "<<G4BestUnit(thisRunTotEM/numEvents,"Energy")<<G4endl;
	G4cout<<"\t Average energy in Had calo: "<<G4BestUnit(totHadCalo/numEvents,"Energy")<<G4endl;
	//for ( int layer = 0 ; layer < NUMLAYERS ; ++layer)
	//{
	//	G4cout<<"\t\t Average energy in Layer "<<layer<<": "<<G4BestUnit(thisRunTotHad[layer],"Energy")<<G4endl;
	//}
	G4cout<<"\t Average number of gammas: "<<thisRunNumGammas/numEvents<<G4endl;
	G4cout<<"\t Average number of neutrons: "<<thisRunNumNeutrons/numEvents<<G4endl;
	G4cout<<"================="<<G4endl;
}
