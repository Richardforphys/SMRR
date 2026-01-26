#ifndef PhysicsList_h
#define PhysicsList_h 1

/*!
\file
\brief defines mandatory user class PhysicsList
*/

#include "G4VUserPhysicsList.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/*!
\brief This mandatory user class provides the physics

It is responsible for
 - Definition of particles
 - Construction of physics processes
 - setting of user cuts

\sa ConstructParticle(), ConstructProcess(), SetCuts()
*/
class PhysicsList: public G4VUserPhysicsList
{
public:
  //! Constructor
  PhysicsList();
  //! Destructor
  ~PhysicsList();

protected:
  //! Construct particle and physics (mandatory)
  //@{
  //! Construct particles
  void ConstructParticle();
  //! Construct physics processes
  void ConstructProcess();
  //! Define user cuts
  void SetCuts();
  //@}
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

 
