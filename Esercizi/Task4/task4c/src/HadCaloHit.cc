// $Id: HadCaloHit.cc 100 2010-01-26 16:12:59Z adotti $
/**
 * @file
 * @brief Implementation of user class HadCaloHit.
*/

#include "HadCaloHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
// -- one more nasty trick for new and delete operator overloading:
G4Allocator<HadCaloHit> HadCaloHitAllocator;

HadCaloHit::HadCaloHit(const G4int layer) :
		layerNumber(layer),
		eDep(0),
		position(G4ThreeVector())
{
}

HadCaloHit::~HadCaloHit()
{
}

void HadCaloHit::Print()
{
	//---------------------
	// Exercise 1 of task 4c
	//---------------------
	//Add some print out on screen: layer number and energy deposited

  //G4cout << "Energy Deposited in layer " << layerNumber << "is " << G4BestUnit(eDep, "Energy") << G4endl;
  //G4cout << "Position: " << position << G4endl;
}

void HadCaloHit::Draw()
{
    G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
    if(pVVisManager)
    {
        G4Circle circle(position); // Instantiate a circle with its 3D
                                // position. The argument "position"
                                // is defined as G4Point3D instance
        circle.SetScreenDiameter (5.0); // Should be circle.SetScreenDiameter
                                        //  (1.0 * pixels) - to be implemented
        circle.SetFillStyle (G4Circle::filled); // Make it a filled circle
        G4Colour colour(0.6,0.3,0.9);              // Define red color
        G4VisAttributes attribs(colour);        // Define a red visualization attribute
        circle.SetVisAttributes(attribs);       // Assign the red attribute to the circle
        pVVisManager->Draw(circle);

    }
}