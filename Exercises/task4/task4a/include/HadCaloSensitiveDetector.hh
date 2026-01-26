// $Id: HadCaloSensitiveDetector.hh 22 2009-12-22 12:36:46Z schaelic $

#ifndef HadCaloSensitiveDetector_h
#define HadCaloSensitiveDetector_h 1

/**
 * @file
 * @brief Defines HadCaloSensitiveDetector class.
 */

#include "G4VSensitiveDetector.hh"

class G4Step;
class G4TouchableHistory;
class G4HCofThisEvent;           // <<- means "H(it) C(ollections) of This Event"


/*!
 * \brief Defines sensitve part of HAD calo detector geometry.
 *
 * Stores Hits with 
 *  * deposited energy
 *  * position
 * in <i>Hit Collections of This Event</i>
 *
 * /sa ProcessHits()
 */
class HadCaloSensitiveDetector : public G4VSensitiveDetector
{
public:
  /// Constructor
  HadCaloSensitiveDetector(G4String SDname);
  /// Destructor
  ~HadCaloSensitiveDetector();

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
};

#endif
