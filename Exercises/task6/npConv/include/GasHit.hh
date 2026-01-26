// $Id: HadCaloHit.hh 100 2010-01-26 16:12:59Z adotti $
#ifndef GasHit_h
#define GasHit_h 1

/**
 * @file
 * @brief Define user class HadCaloHit.
 *
 * We define "our" hit content : the layer number and the energy deposited in the layer
 */

#include "G4VHit.hh"
#include "G4Allocator.hh"
#include "G4THitsCollection.hh"
#include "TFile.h"
#include "TH1F.h"

/*!
 * \brief This class stores information of a hit.
 *
 * It contains
 *  - Hadronic calorimeter layer number
 *  - Deposited energy in layer
 */
class GasHit : public G4VHit {
public:
  /// Constructor
  GasHit(const G4int layer);
  /// Destructor
  ~GasHit();
  //! Print on screen a Hit
  void Print();
  
public:
  //! \name The new and delete operators are overloaded for performances reasons:
  /*! -- Tricky business here... :-(, but provided for you below */
  //@{
  inline void *operator    new(size_t);
  inline void  operator delete(void *aHit);
  //@}

public:
  //! \name  simple set and get methods
  //@{
  void          AddEdep(const double e){ eDep += e; }

  G4double      GetEdep()        const { return eDep;}
  G4int         GetLayerNumber() const { return layerNumber; }
  //@}


  
    

private:
  const G4int   layerNumber;
  G4double      eDep;
};

// Define the "hit collection" using the template class G4THitsCollection:
typedef G4THitsCollection<GasHit> GasHitCollection;


// -- new and delete overloaded operators:
extern G4Allocator<GasHit> GasHitAllocator;

inline void* GasHit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) GasHitAllocator.MallocSingle();
  return aHit;
}
inline void GasHit::operator delete(void *aHit)
{
  GasHitAllocator.FreeSingle((GasHit*) aHit);
}

#endif
