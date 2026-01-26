#ifndef LABR3_HIT_HH
#define LABR3_HIT_HH

#include "G4VHit.hh"
#include "G4Allocator.hh"
#include "G4THitsCollection.hh"
#include "G4ThreeVector.hh"

#include "TFile.h"
#include "TH1F.h"

class LaBr3Hit : public G4VHit {

public:
    LaBr3Hit(const G4int layer = 0);
    virtual ~LaBr3Hit();

    void Print() override;

    inline void* operator new(size_t);
    inline void  operator delete(void* hit);

    void AddEdep(const double e) { eDep += e; }
    G4double GetEdep() const { return eDep; }

    G4int GetLayerNumber() const { return layerNumber; }

private:
    G4int    layerNumber;
    G4double eDep{0.0};
};

// --------------------------------------------------------------------
// Hit collection typedef
// --------------------------------------------------------------------
typedef G4THitsCollection<LaBr3Hit> LaBr3HitCollection;

// allocator declaration
extern G4Allocator<LaBr3Hit> LaBr3HitAllocator;

// --------------------------------------------------------------------
// inline new/delete
// --------------------------------------------------------------------
inline void* LaBr3Hit::operator new(size_t)
{
    return (void*)LaBr3HitAllocator.MallocSingle();
}

inline void LaBr3Hit::operator delete(void* hit)
{
    LaBr3HitAllocator.FreeSingle((LaBr3Hit*)hit);
}

#endif
