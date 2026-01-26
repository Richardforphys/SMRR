// $Id: HadCaloHit.hh 100 2010-01-26 16:12:59Z adotti $
#ifndef HadCaloHit_h
#define HadCaloHit_h 1

/**
 * @file
 * @brief Define user class HadCaloHit.
 *
 * We define "our" hit content : the layer number and the energy deposited in the layer
 */

#include "G4VHit.hh"
#include "G4Allocator.hh"
#include "G4THitsCollection.hh"

/*!
 * \brief This class stores information of a hit.
 *
 * It contains
 *  - Hadronic calorimeter layer number
 *  - Deposited energy in layer
 */
class HadCaloHit : public G4VHit {
public:
  /// Constructor
  HadCaloHit(const G4int layer);
  /// Destructor
  ~HadCaloHit();
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
typedef G4THitsCollection<HadCaloHit> HadCaloHitCollection;


// -- new and delete overloaded operators:
extern G4Allocator<HadCaloHit> HadCaloHitAllocator;

inline void* HadCaloHit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) HadCaloHitAllocator.MallocSingle();
  return aHit;
}
inline void HadCaloHit::operator delete(void *aHit)
{
  HadCaloHitAllocator.FreeSingle((HadCaloHit*) aHit);
}

#endif
