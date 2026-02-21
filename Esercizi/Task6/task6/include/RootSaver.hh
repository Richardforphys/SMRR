// $Id: RootSaver.hh 47 2010-01-19 16:20:46Z adotti $
/**
 * @file   RootSaver.hh
 *
 * @date   17 Dec 2009
 * @author gcroci
 *
 * @brief  Save hits in a ROOT TTree
 */

#ifndef ROOTSAVER_HH_
#define ROOTSAVER_HH_

#include <string>
#include <TTree.h>
#include <TFile.h>
#include "G4UnitsTable.hh"
#include "GasHit.hh"
#include "TrackParticle.hh"
#include "SensitiveDetector.hh"
#include "EventAction.hh"
#include "TrackParentParticle.hh"
#include "G4Types.hh"

class TFile;

/*!
 * \brief Save hits and digits to a ROOT TTree.
 *
 * This class can be used to save in a TTree hits
 * and digits.
 * The TTree structure is described below.
 */
class RootSaver
{
public:

  
  
  //! Default constructor,
  RootSaver();
  //! Default destructor.
  virtual ~RootSaver();
  /*! \brief Open a TFile and create ROOT TTree.
   *
   * Each time that a /run/beamOn command is issued
   * this method should be called
   * Every time this method is called the run counter
   * is incremented and the file name is modified accordingly:
   * tree_run<n>.root
   * \sa CloseTree()
   * @param fileName : The ROOT file name prefix
   * @param treeName : The name of the TTree
   */
  typedef std::map<G4int,TrackParticle*> trackMap_t;
  
  //typedef std::map<G4int,TrackParentParticle*> trackParentMap_t;
  
  virtual void CreateTree( const std::string& fileName = "tree" , 
				 const std::string& treeName = "Gas_Tree" );
  /*! Close the file and save ROOT TTree
   * The ROOT file should be closed at the end of each /run/beamOn
   * \sa CloseTree
   */
  virtual void CloseTree();
  //! Add hits and digi container for this event
  virtual void AddEvent(trackMap_t tMap,G4int evID);

  
private:

  TFile* rootFile;
  TTree* rootTree; //!< Pointer to the ROOT TTree
  unsigned int runCounter; //!< Run counter to uniquely identify ROOT file

  //! \name TTree variables
  //@{
  //! Number of strips of each module
  
  
  //Number of tracks in one Event
  

  Int_t Tot_Tracks;
  //
  //Each number is associated to a type of particle
  //From the particle it is possible to understand which particle was created
  
  //Particle ID
  Int_t* PartID;
  Int_t* Part_Moth_ID;
  Int_t* PartType;
  Int_t* MothPartType;
  //Energy Deposited
  Float_t* Part_EnDep;
  Float_t* MothPart_EnDep;
  //Z Starting Position
  Float_t* Part_zStart;
  Float_t* MothPart_zStart;
  //Start Time
  Float_t* Part_tStart;
  Float_t* MothPart_tStart;
  
  //Event ID
  Int_t Event_ID;

  
  
  
  
  
  //};


  //typedef std::map<G4int,TrackParticle*> trackMap_t;
  
  typedef std::map<G4int,TrackParentParticle*> trackParentMap_t;


//typedef std::map<G4int,TrackParticle*> trackMap_t;

//typedef std::map<G4int,TrackParentParticle*> trackParentMap_t;

  trackParentMap_t GetParentProperties(trackMap_t TParticleMap); 

};

#endif /* ROOTSAVER_HH_ */

