#include "G4VHit.hh"
#include "G4Allocator.hh"
#include "G4THitsCollection.hh"
// ROOT libs
#include "TFile.hh"
#include "TH1F.hh"

class LaBr3Hit : public G4VHit {
    public:
        LaBr3Hit(const G4int layer);
        ~LaBr3Hit();

        //My specific hit interface
        void Print();
        void AddEdep(const double e) {return eDep += e};
        G4double GetEdep() {return eDep;}
        G4int GetLayerNumber() {return layerNumber;}

    private:
        G4int layerNumber;
        G4double eDep;

        // Operators overload
        inline void *operator new(size_t); //Ridefinisco l'operatore new
        inline void *operator delete(void *aHit); //Ridefinisco l'operatore delete 
        // per un puntatore a void
}

//Define the hit collection using the template class G4THitsCollection.hh
typedef G4THitsCollection<LaBr3Hit> LaBr3HitCollection;
//Definisco allocatori di memoria che ne occuperanno quanta ne serve per hit LaBr3
extern G4Allocator<LaBr3Hit> LaBr3HitAllocator; 
//Specifico cosa fa operator new
inline void* LaBr3Hit::operator new(size_t) {
    void *aHit;
    //Restituisce puntatore void con tanta memoria quanto ne occupa un allocator<LaBr3>
    aHit = (void*) LaBr3HitAllocator.MallocSingle(); 
}

inline void LaBr3Hit::operator delete(void * aHit) {
    LaBr3HitAllocator.FreeSingle((LaBr3Hit*) aHit);
}