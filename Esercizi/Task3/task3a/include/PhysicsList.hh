// $Id: PhysicsList.hh 22 2009-12-22 12:36:46Z schaelic $

/**
 * @file
 * @brief Defines mandatory user class PhysicsList.
*/

#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VUserPhysicsList.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4VPhysicsConstructor;

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
  virtual ~PhysicsList();

protected:
  //! Construct particle and physics (mandatory)
  //@{
  //! Construct particles
  virtual void ConstructParticle();
  //! Construct physics processes
  virtual void ConstructProcess();
  //! Define user cuts
  virtual void SetCuts();
  //@}
private:
 
  G4VPhysicsConstructor*  emPhysicsList;
 
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

 
