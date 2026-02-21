/*
 * @file Analysis.hh
 *
 * @date: 9 Feb 2010
 * @author: adotti
 *
 * \brief Analysis class
 */

#ifndef ANALYSIS_HH_
#define ANALYSIS_HH_

#include "G4Event.hh"
#include "G4Run.hh"

#define NUMLAYERS 80

/*!
 * \brief Analysis class
 * This class contains the code to collect information from
 * the different UserActions.
 * The class is designed as a singleton.
 * To access it you need to use:
 * Analysis* analysis = Analysis::GetInstance()
 */
class Analysis {
public:
	//! Singleton pattern
	static Analysis* GetInstance() {
		if ( Analysis::singleton == NULL ) Analysis::singleton = new Analysis();
		return Analysis::singleton;
	}
	//! destructor
	virtual ~Analysis() {};
	//! Should be called at the beginning of an event
	void PrepareNewEvent(const G4Event* anEvent);
	//! Should be called at the end of an event
	void EndOfEvent(const G4Event* anEvent);
	//! Should be called at the beginning of a run
	void PrepareNewRun(const G4Run* aRun);
	//! Should be called at the end of a run
	void EndOfRun(const G4Run* aRun);
	//! Add up some energy deposit in EM calorimeter
	void AddEDepEM( G4double edep ) { thisEventTotEM += edep; }
	//! Increase number of secondaries
	void AddSecondary( G4int num ) { thisEventSecondaries += num; }
private:
	//! Private construtor: part of singleton pattern
	Analysis();
	//! Singleton static instance
	static Analysis* singleton;
	//! Tot energy for this event in EM calo
	G4double thisEventTotEM;
	//! Number of secondaries for this event
	G4int thisEventSecondaries;
	//! Tot energy for this run in EM calo
	G4double thisRunTotEM;
	//! Number of secondaries for this run
	G4int thisRunTotSecondaries;
	//! Array of energy in each layer of HAD calo for this event
	G4double thisEventTotHad[NUMLAYERS];
	//! Array of energy in each layer of HAD calo for this run
	G4double thisRunTotHad[NUMLAYERS];
};

#endif /* ANALYSIS_HH_ */
