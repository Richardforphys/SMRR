///Authors gcroci, malfonsi
//This Class defines the properties of a particle
//like 
//
//
//1) The Particle Type
//2) The Particle ID
//3) The total particle energy deposition
//4) The mother particle ID

#ifndef TRACKPARTICLE_HH_
#define TRACKPARTICLE_HH_

#include "GasHit.hh"
#include "G4UnitsTable.hh"
//#include "G4ThreeVector.hh"


class TrackParticle
{

public:

  //constructor
  TrackParticle();
  //destroyer
  ~TrackParticle();
  

private:

  G4int Part_Type;
  G4double Tot_Edep;
  G4int Moth_Part_ID;
  G4String Part_name;
  G4double ZStart_Pos;
  G4double Start_Time;
  //G4ThreeVector End_Pos;
  

public:
  
  G4double TrackAddEDep(G4double e);

  //setter

  void SetPart_Type(G4int ptype);
  void SetMothPart_ID(G4int mpID);
  void SetEdep(G4double ed);
  void SetPart_name(G4String pname);
  void SetZStart_Pos(G4double zstpos);
  void SetStart_Time(G4double stime);
  //void SetEnd_Pos(G4ThreeVector endpos);
  
  


  //getter
  G4int GetPart_Type() {return Part_Type;}
  G4double GetTot_Edep() {return Tot_Edep;}
  G4int GetMoth_Part_ID() {return Moth_Part_ID;}
  G4String GetPart_name() {return Part_name;}
  G4double GetZStart_Pos() {return ZStart_Pos;}
  G4double GetStart_Time() {return Start_Time;}
  //G4ThreeVector GetEnd_Pos() {return End_Pos};

};

#endif
