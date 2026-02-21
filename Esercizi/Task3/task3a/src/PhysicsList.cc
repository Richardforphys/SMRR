// $Id: PhysicsList.cc 98 2010-01-26 15:59:09Z vnivanch $
/**
 * @file
 * @brief Implements mandatory user class PhysicsList.
 */

#include "globals.hh"
#include "PhysicsList.hh"

#include "G4EmStandardPhysics.hh"
#include "G4LossTableManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleTypes.hh"

PhysicsList::PhysicsList():  G4VUserPhysicsList()
{
  defaultCutValue = 10.0*um;
  emPhysicsList = new G4EmStandardPhysics();
  SetVerboseLevel(1);
}

PhysicsList::~PhysicsList()
{}

void PhysicsList::ConstructParticle()
{
  G4Geantino::GeantinoDefinition();
  G4ChargedGeantino::ChargedGeantinoDefinition();
  emPhysicsList->ConstructParticle();
}

void PhysicsList::ConstructProcess()
{
  AddTransportation();
  emPhysicsList->ConstructProcess();
}

void PhysicsList::SetCuts()
{
  SetCutsWithDefault();
  if (verboseLevel>0) { DumpCutValuesTable(); }
}

