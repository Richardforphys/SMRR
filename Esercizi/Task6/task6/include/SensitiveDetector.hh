// $Id: SensitiveDetector.hh 22 2009-12-22 12:36:46Z schaelic $

#ifndef SensitiveDetector_h
#define SensitiveDetector_h 1

/**
 * @file
 * @brief Defines SensitiveDetector class.
 */

#include "G4VSensitiveDetector.hh"
#include "GasHit.hh" //our hit format
//#include "TrackingAction.hh"
//#include "TrackInformation.hh"
//#include "TrackingAction.hh"
#include "G4SystemOfUnits.hh"

//our homemade class

#include "TrackParticle.hh" 
#include "TrackParentParticle.hh"

class DetectorConstruction;
class RunAction;



class G4Step;
class G4TouchableHistory;
class G4HCofThisEvent;           // <<- means "H(it) C(ollections) of
				 // This Event" 


/*!
 * \brief Defines sensitve part of detector geometry.
 *
 * Stores Hits with 
 *  * deposited energy
 *  * position
 * in <i>Hit Collections of This Event</i>
 *
 * /sa ProcessHits()
 */
class SensitiveDetector : public G4VSensitiveDetector
{
public:
  /// Constructor
  SensitiveDetector(G4String SDname);
  /// Destructor
  ~SensitiveDetector();

public:
  /// @name methods from base class G4VSensitiveDetector
  //@{
  /// Mandatory base class method : it must to be overloaded:
   G4bool ProcessHits(G4Step *step, G4TouchableHistory *ROhist);

  /// (optional) method of base class G4VSensitiveDetector
  void Initialize(G4HCofThisEvent* HCE);
  /// (optional) method of base class G4VSensitiveDetector
  void EndOfEvent(G4HCofThisEvent* HCE);

  //@}

  
private:
  typedef std::map<G4int,GasHit*> hitMap_t; //< Helper mapping layer number with hit
  hitMap_t hitMap;
  
  GasHitCollection* hitCollection;          //< Collection of hits in the gas

  typedef std::map<G4int,TrackParticle*> trackMap_t;
  trackMap_t trackMap;

  typedef std::map<G4int,TrackParentParticle*> trackParentMap_t;
  trackParentMap_t trackParentMap;



  //map getter

public:
  
trackMap_t GetTrackMap() {return trackMap;}

};
  #endif
