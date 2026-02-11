// $Id: HadCaloHit.cc 100 2010-01-26 16:12:59Z adotti $
/**
 * @file
 * @brief Implementation of user class HadCaloHit.
*/

#include "GasHit.hh"
#include "G4UnitsTable.hh"

// -- one more nasty trick for new and delete operator overloading:
G4Allocator<GasHit> GasHitAllocator;

GasHit::GasHit(const G4int layer) :
		layerNumber(layer),
		eDep(0)
		
{
  
  
}

GasHit::~GasHit()
{
  
}



void GasHit::Print()
{
	//---------------------
	// Exercise 1 of task 4c
	//---------------------
	//Add some print out on screen: layer number and energy deposited
  ///GABRIELE

  //HadCaloHit 
  
  //G4double edep = this.GetEdep();
  //G4int layern = this.GetLayerNumber();

  
  
  //G4cout << "HIT>>>>>>>>>>>>>>>>>>> Energy deposited: "  << G4BestUnit(eDep,"Energy") << " in layer " << layerNumber << G4endl;

  
  
  

}
