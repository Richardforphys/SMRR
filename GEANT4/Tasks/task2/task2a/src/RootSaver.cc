// $Id: RootSaver.cc 94 2010-01-26 13:18:30Z adotti $
/**
 * @file   RootSaver.cc
 *
 * @date   17 Dec 2009
 * @author adotti
 * 
 * @brief  Implements class RootSaver.
 */

#include "RootSaver.hh"
#include "SiDigi.hh"
#include "SiHit.hh"
#include "TTree.h"
#include "TFile.h"
#include "TMath.h"
#include <sstream>
#include <iostream>
#include <cassert>

RootSaver::RootSaver() :
	rootTree(0),
	runCounter(0),
	nStrips(0),
	Signal1(0),
	Signal2(0),
	Signal3(0),
	TruthPos1(0),
	TruthPos2(0),
	TruthPos3(0),
	TruthE1(0),
	TruthE2(0),
	TruthE3(0),
	TruthPos0(0),
	TruthAngle0(0)
{
}

RootSaver::~RootSaver()
{
	//Close current file if needed
	if ( rootTree )
	{
		CloseTree();
	}
}

void RootSaver::CreateTree( const std::string& fileName , const std::string& treeName )
{
	if ( rootTree )
	{
		std::cerr<<"TTree already created, first call CloseTree"<<std::endl;
		return;
	}
	std::ostringstream fn;
	fn << fileName << "_run" << runCounter++ << ".root";
	//Create a new file and open it for writing, if the file already exists the file
	//is overwritten
	TFile* rootFile = TFile::Open( fn.str().data() , "recreate" );
	if ( rootFile == 0 || rootFile->IsZombie() )
	{
		G4cerr<<"Error opening the file: "<<fn.str() <<" TTree will not be saved."<<G4endl;
		return;
	}
	rootTree = new TTree( treeName.data() , treeName.data() );
	//TODO: Get detector strip numbers automatically
	nStrips = 48;
	Signal1 = new Float_t[nStrips];
	Signal2 = new Float_t[nStrips];
	Signal3 = new Float_t[nStrips];
	for ( Int_t strip = 0 ; strip < nStrips ; ++strip )
	{
		Signal1[strip] = 0;
		Signal2[strip] = 0;
		Signal3[strip] = 0;
	}
	//Digits variables
	rootTree->Branch( "signal1", Signal1 , "signal1[48]/F");
	rootTree->Branch( "signal2", Signal2 , "signal2[48]/F");
	rootTree->Branch( "signal3", Signal3 , "signal3[48]/F");
	//Hits variables
	rootTree->Branch( "truthPos1" , &TruthPos1 );
	rootTree->Branch( "truthPos2" , &TruthPos2 );
	rootTree->Branch( "truthPos3" , &TruthPos3 );
	rootTree->Branch( "truthE1" , &TruthE1 );
	rootTree->Branch( "truthE2" , &TruthE2 );
	rootTree->Branch( "truthE3" , &TruthE3 );
	rootTree->Branch( "truthPos0" , &TruthPos0 );
	rootTree->Branch( "truthAngle0" , &TruthAngle0 );
}

void RootSaver::CloseTree()
{
	//Check if ROOT TTree exists,
	//in case get the associated file and close it.
	//Note that if a TFile goes above 2GB a new file
	//will be automatically opened. We have thus to get,
	//from the TTree the current opened file
	if ( rootTree )
	{
		G4cout<<"Writing ROOT TTree: "<<rootTree->GetName()<<G4endl;
		//rootTree->Print();
		rootTree->Write();
		TFile* currentFile = rootTree->GetCurrentFile();
		if ( currentFile == 0 || currentFile->IsZombie() )
		{
			G4cerr<<"Error closing TFile "<<G4endl;
			return;
		}
		currentFile->Close();
		//The root is automatically deleted.
		rootTree = 0;
		delete[] Signal1;
		delete[] Signal2;
		delete[] Signal3;
	}
}

void RootSaver::AddEvent( const SiHitCollection* const hits, const SiDigiCollection* const digits ,
						  const G4ThreeVector& primPos, const G4ThreeVector& primMom )
{
	//If root TTree is not created ends
	if ( rootTree == 0 )
	{
		return;
	}
	//Store Digits information
	if ( digits )
	{
		G4int nDigits = digits->entries();
		for ( G4int d = 0 ; d<nDigits ; ++d )
		{
			const SiDigi* digi = static_cast<const SiDigi*>( digits->GetDigi( d ) );
			G4int stripNum = digi->GetStripNumber();
			//Safety check
			if ( stripNum >= nStrips )
			{
				G4cerr<<"Digi Error: Strip number "<<stripNum<<" expected max value:"<<nStrips<<G4endl;
				continue;//Go to next digit
			}
			G4int planeNum = digi->GetPlaneNumber();
			if ( planeNum == 0 )
			{
				Signal1[ stripNum ] = static_cast<Float_t>(digi->GetCharge());
			}
			else if ( planeNum == 1 )
			{
				Signal2[ stripNum ] = static_cast<Float_t>(digi->GetCharge());
			}
			else if ( planeNum == 2 )
			{
				Signal3[ stripNum ] = static_cast<Float_t>(digi->GetCharge());
			}
			else
			{
				G4cerr<<"Digi Error: Plane number "<<planeNum<<" expected max value: 2"<<G4endl;
			}
		}
	}
	else
	{
		G4cerr<<"Error: No digi collection passed to RootSaver"<<G4endl;
	}

	//Store Hits infromation
	if ( hits )
	{
		G4int nHits = hits->entries();
		//Set defaults
		TruthE1 = 0;
		TruthE2 = 0;
		TruthE3 = 0;
		TruthPos1 = 0;
		TruthPos2 = 0;
		TruthPos3 = 0;
		//Loop on all hits, consider only the hits with isPrimary flag
		//Position is weighted average of hit x()
		for ( G4int h = 0 ; (h<nHits) ; ++h )
		{
			const SiHit* hit = static_cast<const SiHit*>( hits->GetHit( h ) );
			//Uncomment this line if you want to record only
			//primary energy depositions
			//if ( hit->GetIsPrimary() == false ) continue;
			G4int planeNum = hit->GetPlaneNumber();
			G4ThreeVector pos = hit->GetPosition();
			G4double x = pos.x();
			//We save x in mm (world coordinates)
			x /= mm;
			//We save energy in MeV
			Float_t edep = static_cast<Float_t>(hit->GetEdep());
			edep /= MeV;
			if ( planeNum == 0 )
			{
				if ( hit->GetIsPrimary() == true ) TruthPos1 = x;
				TruthE1 += edep;
			}
			else if ( planeNum == 1)
			{
				if ( hit->GetIsPrimary() == true ) TruthPos2 = x;
				TruthE2 += edep;
			}
			else if ( planeNum == 2)
			{
				if ( hit->GetIsPrimary() == true ) TruthPos3 = x;
				TruthE3 += edep;
			}
			else
			{
				G4cerr<<"Hit Error: Plane number "<<planeNum<<" expected max value: 2"<<G4endl;
				continue;
			}

		}
	}
	else
	{
		G4cerr<<"Error: No hits collection passed to RootSaver"<<G4endl;
	}
	TruthPos0 = static_cast<Float_t>( primPos.x() );
	//Measure angle of the beam in xz plane measured from z+ direction
	// -pi<Angle<=pi (positive when close to x positiove direction)
	Float_t sign_z = ( primMom.z()>= 0 ) ? +1 : -1;
	Float_t sign_x = ( primMom.x()>= 0 ) ? +1 : -1;
	TruthAngle0 = ( primMom.z() != 0 ) ?
			TMath::PiOver2()*sign_x*(1-sign_z)+std::atan( primMom.x()/primMom.z() )
			: sign_x*TMath::PiOver2(); //beam perpendicular to z
	TruthAngle0 /= mrad;
	rootTree->Fill();
}
