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
#include "GasHit.hh"
#include "TTree.h"
#include "TFile.h"
#include "TMath.h"
#include <sstream>
#include <iostream>
#include <cassert>
#include "G4Types.hh"
#include "TrackParentParticle.hh"
#include "G4UnitsTable.hh"

RootSaver::RootSaver() :
	rootFile(0),
	rootTree(0),
	runCounter(0),
	Tot_Tracks(0)
{

  PartID = new Int_t[10000];
  Part_Moth_ID = new Int_t[10000];
  PartType = new Int_t[10000];
  MothPartType = new Int_t[10000];
  Part_EnDep = new Float_t[10000];
  MothPart_EnDep = new Float_t[10000];
  Part_zStart = new Float_t[10000];
  MothPart_zStart = new Float_t[10000];
  Part_tStart = new Float_t[10000];
  MothPart_tStart = new Float_t[10000];
  

}

RootSaver::~RootSaver()
{
	//Close current file if needed
	if ( rootTree )
	{
		CloseTree();
	}
}

void RootSaver::CreateTree( const std::string& fileName , 
			    const std::string& treeName )
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
	rootFile = TFile::Open( fn.str().data() , "recreate" );
	if ( rootFile == 0 || rootFile->IsZombie() )
	{
		G4cerr<<"Error opening the file: "<<fn.str() <<" TTree will not be saved."<<G4endl;
		return;
	}
	else
	  {
	    G4cout << "rootFile correctly open" << G4endl;
	    G4cout << " NAME of the File: " << rootFile->GetName() << G4endl;
	  }
	
	rootTree = new TTree( treeName.data() , treeName.data() );
	
	//Define Branches for the tree
	rootTree->Branch( "ntracks" , &Tot_Tracks, "ntracks/I2" );
	rootTree->Branch( "id", PartID , "id[ntracks]/I2");
	rootTree->Branch( "mum", Part_Moth_ID  , "mum[ntracks]/I2");
	rootTree->Branch( "type", PartType , "type[ntracks]/I");
	rootTree->Branch( "mtype",  MothPartType  , "mtype[ntracks]/I2");
	rootTree->Branch( "edep", Part_EnDep  , "edep[ntracks]/F");
	rootTree->Branch( "medep",  MothPart_EnDep , "medep[ntracks]/F");
	rootTree->Branch( "zp", Part_zStart  , "zp[ntracks]/F");
	rootTree->Branch( "mzp", MothPart_zStart  , "mzp[ntracks]/F");
	rootTree->Branch( "t", Part_tStart  , "t[ntracks]/F");
	rootTree->Branch( "mt", MothPart_tStart  , "mt[ntracks]/F");
	rootTree->Branch("evid",&Event_ID,"evid/I2");
}

void RootSaver::CloseTree()
{

  //TFile* currentFile = new TFile("TreeFile.root","RECREATE"); 
  //Check if ROOT TTree exists,
  //in case get the associated file and close it.
  //Note that if a TFile goes above 2GB a new file
  //will be automatically opened. We have thus to get,
  //from the TTree the current opened file
  if ( rootTree )
    {
      G4cout<<"Writing ROOT TTree: "<<rootTree->GetName()<<G4endl;
      rootTree->Print();
      
      rootFile->ReOpen("Update");
    
      if (rootTree->Write() !=0)
	{
	  G4cout << "TTree correctly written in the file" << G4endl;
	}

      else
	{
	  G4cout << "Problem writing in the file" << G4endl;
	}
      
      //TFile* currentFile = rootTree->GetCurrentFile();
      // if ( currentFile == 0 || currentFile->IsZombie() )
// 	{
// 	  G4cerr<<"Error closing TFile "<<G4endl;
// 	  return;
      //}
      //currentFile->Close();
      rootFile->Close();
      //The root is automatically deleted.
      rootTree = 0;
      
      delete[] PartID;
      delete[] PartType;
      delete[] Part_EnDep;
      delete[] Part_Moth_ID;
      delete[] Part_zStart;
      delete[] Part_tStart;
      
    }
}

void RootSaver::AddEvent(trackMap_t trackMap, G4int evID )
{
  //If root TTree is not created ends
  if ( rootTree == 0 )
    {
      return;
    }
  
  Event_ID = evID;

  //G4cout << ">>>>>>>>>>>>>>>Adding Event Number: " << Event_ID 
  //	 << "<<<<<<<<<<<<<<<" << G4endl;

  trackParentMap_t trackParentMap = this->GetParentProperties(trackMap);
  
  Tot_Tracks = trackParentMap.size();

  //G4cout << "Total Tracks= " << Tot_Tracks << G4endl;
  
  trackParentMap_t::iterator itt = trackParentMap.begin();
  trackParentMap_t::iterator itt_end = trackParentMap.end();
  Int_t i=0;
  
  
  if(Tot_Tracks < 10000) 
    {
      for ( ; itt != itt_end ; ++itt)
	{

	//G4cout << "\n" ;

 	/// G4cout << "!!!!!!!!!!!!!!!!ROOT SAVER " 
 	/// 	 << "Energy Deposited by particle ID "
 	/// 	 << itt->first 
 	/// 	 << " that is a " << (itt->second)->GetPart_name() << "  is  " 
 	/// 	 << G4BestUnit( (itt->second)->GetTot_Edep(),"Energy") 
 	/// 	 << G4endl;
	  
	  

	  PartID[i] = itt->first;
	  Part_Moth_ID[i] = ( itt->second ) ->GetMoth_Part_ID() ;
	  PartType[i] = (itt->second)->GetPart_Type();
	  MothPartType[i] = (itt->second)->GetMothPart_Type();
	  //G4cout << "PartType[" << i << "] = " << PartType[i] << G4endl;
	  Part_EnDep[i] = static_cast<Double_t>( ( (itt->second)->GetTot_Edep() )/ MeV ) ;
	  MothPart_EnDep[i] =  static_cast<Double_t>( (itt->second)->GetMothTot_Edep()  / MeV );
	  Part_zStart[i] =   ( (itt->second )  ->GetZStart_Pos() / mm ); 
	  MothPart_zStart[i] = ( (itt->second)->GetMothZStart_Pos()  / mm ) ;
	  Part_tStart[i] = ( (itt->second)->GetStart_Time() ) / ns;
	  MothPart_tStart[i] = (itt->second)->GetMothStart_Time() / ns;
	  
	  // G4cout << "PartID[" << i << "] = " << PartID[i] 
// 		 << "  Energy Deposition = " << Part_EnDep[i] << G4endl ;

	  //DO NOT SAVE PRIMARY PARTICLES THAT HAVE NOT DEPOSITED ANY ENERGY!!!!
	  //if (Part_EnDep[i] > 0 || PartID[i] > 1)   i++;
	  i++;
	}
      
      //SINCE WE DECIDED TO NOT SAVE PRIMARY PARTICLES THAT DO NOT
      //DEPOSIT ANY ENERGY, WE HAVE TO CORRECT THE NUMBER OF
      //*RECORDED* TRACKS
      Tot_Tracks = i;
      
      // G4cout << "Total Tracks: " << i << G4endl;
      
      if (Tot_Tracks > 0) rootTree->Fill();
    }
  else
    {
      G4cout << "Number of tracks (" << Tot_Tracks << ") exceed maximum limit in event " 
	     << Event_ID << " ---> Event not saved in the Tree" << G4endl;
    }
  
}

typedef std::map<G4int,TrackParentParticle*> trackParentMap_t;

trackParentMap_t RootSaver::GetParentProperties(trackMap_t TParticleMap)
{
  //DEFINE ENTRIES TO BE STORED IN EACH TRACKPARENTPARTICLE OBJECT
  
  G4int trackID=0;
  
  G4int MothtrackID=0;
  
  G4int ptype=0;

  G4double edep=0;

  G4double zp=0;
  
  G4double zs=0;

  G4String pname = "";

  G4int pptype=0;
   
  G4double pedep=0;

  G4double pzp=0;
  
  G4double pzs=0;

  G4String ppname = "";

  G4int IntArray[3] = {0,0,0};

  G4double DoubleArray[6] = {0,0,0,0,0,};

  //DEFINE ITERATOR TO THE TRACKPARTICLEMAP PASSED IN THE FUNCTION
  
  trackMap_t::iterator it = TParticleMap.begin();
  
  //DEFINE AN OBJECT OF THE CLASS TRACKPARENTMAP FOR SUCCESSIVE STORING

  trackParentMap_t TParentMap;

  //Clear the map
  TParentMap.clear();
  
  //Define iterator pointing at the first element of the trackParentMap_t Map
  trackParentMap_t::iterator parit = TParentMap.begin();
  
  
  for (; it!=TParticleMap.end(); it++)
    {
      TrackParentParticle* track_parent_par = new TrackParentParticle();
      
      trackID = it->first;
      MothtrackID = static_cast<G4int>( ( static_cast<TrackParticle*> (it->second) ) ->GetMoth_Part_ID() );
      
      IntArray[0] = MothtrackID;
      
      //G4cout << "<<<<<<<<< TrackID: " << trackID << "   MotherTrackID: " 
      //       <<  MothtrackID << " >>>>>>>>>>>> " << G4endl;  
      
      //Try to find the element in TParticleMap with trackID==MothertrackID in order to get the parent
      //properties
      trackMap_t::iterator it_tmp = TParticleMap.find(MothtrackID);
      
      //Look if the element with trackID is already present in the TParentMap
      //=> It should not be the case since TParentMap is fresh
      trackParentMap_t::iterator it_tmp_par = TParentMap.find(trackID); 	  
      
      if(it_tmp != TParticleMap.end() )  
  	{
	  
	  //it is used to get the present particle properties while it_tmp is used to get the 
	  //mother particle properties once identified the mother particle
	  
  	  //track_parent_par = (it_tmp_par->second);
  	    
  	  //MothtrackID = ( static_cast<TrackParticle*> (it->second) ) ->GetMoth_Part_ID();
  	  
  	  ptype = ( static_cast<TrackParticle*> (it->second) ) ->GetPart_Type();
	  
	  IntArray[1] = ptype;
	  
	  pptype = ( static_cast<TrackParticle*> (it_tmp->second) ) ->GetPart_Type();
	  
	  IntArray[2] = pptype;
	  
	  edep   = ( static_cast<TrackParticle*> (it->second) ) ->GetTot_Edep();
	  
	  DoubleArray[0] = edep;
	  
	  pedep   = ( static_cast<TrackParticle*> (it_tmp->second) ) ->GetTot_Edep();
	  
	  DoubleArray[1] = pedep;
	  
	  pname   = ( static_cast<TrackParticle*> (it->second) ) ->GetPart_name();
	  
	  ppname   = ( static_cast<TrackParticle*> (it_tmp->second) ) ->GetPart_name();
	  
	  zp = ( static_cast<TrackParticle*> (it->second) ) ->GetZStart_Pos();
	  
	  DoubleArray[2] = zp;
	  
	  pzp = ( static_cast<TrackParticle*> (it_tmp->second) ) ->GetZStart_Pos();
	  
	  DoubleArray[3] = pzp;
	  
  	  zs = ( static_cast<TrackParticle*> (it->second) ) ->GetStart_Time();
	  
  	  DoubleArray[4] = zs;
	  
  	  pzs = ( static_cast<TrackParticle*> (it_tmp->second) ) ->GetStart_Time();
	  
  	  DoubleArray[5] = pzs;
	  
	  bool printvalues = false;

	  if(printvalues)
	    {

	      G4cout << "***********PRESENT PARTICLE*********** \n" << G4endl;
	      G4cout << "Track ID: "  << trackID 
		     << "\t Type: "   << ptype 
		     << "\t Name: "   << pname 
		     << "\t EDep: "   << G4BestUnit(edep,"Energy")  
		     << "\t ZStPos: " << G4BestUnit(zp,"Length")    
		     << "\t TStart: " << G4BestUnit(zs, "Time")    
		     << "\n" << G4endl;
	      
	      G4cout << "***********MOTHER PARTICLE*********** \n" << G4endl;
	      G4cout << "Mother Track ID: "  << MothtrackID  
		     << "\t Mother Type: "   << pptype 
		     << "\t Mother Name: "   << ppname 
		     << "\t Mother EDep: "   << G4BestUnit(pedep,"Energy")  
		     << "\t Mother ZStPos: " << G4BestUnit(pzp,"Length")    
		
		     << "\t Mother TStart: " << G4BestUnit(pzs, "Time")
		     << "\n" << G4endl;
	      
	      G4cout << " \n" << G4endl;
	    }
	  
	  //Set the properties into the temporary TrackParentParticleObject declared at the 
	  //beginning of the for cycle
	  track_parent_par->SetIntValues(IntArray);
  	  track_parent_par->SetDoubleValues(DoubleArray);
      
	  
	  //After setting the int and double values into the TrackParentParticleObject
	  //it is stored in TParentMap
	  if (printvalues) G4cout << "Inserting entry with key/trackID " << trackID 
				  << " in the map" << G4endl;
	  
	  TParentMap.insert( std::make_pair(trackID, track_parent_par) );
	     
	  ///**************OLD**************************************************  
  	  /// //SET ONCE AGAIN PRESENT PARTICLE PROPERTIES
  	  //track_parent_par->SetPart_Type(ptype);  //OK
  	  //track_parent_par->SetMothPart_ID(MothtrackID); //NO 
  	  //track_parent_par->DoubleSet_Type_MumID(ptype,MothtrackID); //NO
      
  	  //track_parent_par->SetEdep(edep); //OK
  	  //track_parent_par->SetPart_name(pname); //NO
  	  //track_parent_par->SetZStart_Pos(zp); //OK
  	  //track_parent_par->SetStart_Time(zs); //OK
  	  
  	  //SET PARENT PARTICLE PROPERTIES
  	  
  	  //track_parent_par->SetMoth_Part_Type(pptype); //OK
  	  //track_parent_par->SetMoth_Edep(pedep);
  	  //track_parent_par->SetMoth_Part_name(ppname);
  	  //track_parent_par->SetMoth_ZStart_Pos(pzp); 
  	  //track_parent_par->SetMoth_Start_Time(pzs);
	  //*********************************************************************
	} 
      
      else
	{
	  bool noentry = false;
	  if(noentry) G4cout << "No Entry in the map with the key/trackID " 
			     << MothtrackID << G4endl;
	}
	  
      bool printvalues_end = false;
      
      if(printvalues_end) G4cout << "Size of the map is " << TParentMap.size() << G4endl;

      //We do not need to delete this object
      //if(track_parent_par) delete track_parent_par;

    }

  bool printmap = false;

  if(printmap)
    {
      G4cout << "Print TParentMap values before returing it \n" << G4endl;

      trackParentMap_t::iterator pmit = TParentMap.begin();
      trackParentMap_t::iterator pmit_end = TParentMap.end();
      

      for( ; pmit!=pmit_end; ++pmit)
	
	{
	  G4int tid = pmit->first;
	  G4double  ed = ( static_cast<TrackParentParticle*> (pmit->second) ) ->GetTot_Edep();
	  G4double ped = ( static_cast<TrackParentParticle*> (pmit->second) ) ->GetMothTot_Edep();
	    


	  G4cout << "Track ID= " << tid
		 << "\n Particle-Edep= " << G4BestUnit(ed,"Energy")
		 << "\n Mother Particle-Edep= " << G4BestUnit(ped,"Energy") 
		 << "\n" << G4endl;
	}
    }

  return TParentMap;
  
}
  	  
  	
