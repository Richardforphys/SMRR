///Authors gcroci, malfonsi 
/// 10/03/2010
///

#include "TrackParentParticle.hh"
//#include "TrackParticle.hh"
#include "TrackParentParticle.hh"
#include "SensitiveDetector.hh"
#include "EventAction.hh"
#include "RunAction.hh"

TrackParentParticle::TrackParentParticle()
{
  Moth_Part_ID = 0;
  Part_Type = 0;
  Moth_Part_Type = 0;
  Tot_Edep = 0.0;
  ZStart_Pos = 0.0;

  bool printvalues = false;
    
  if(printvalues)
    {

      G4cout << "Creating TrackParentParticleObject" << G4endl;
      
      Moth_ZStart_Pos = 0.0;
      
      G4cout << "Initial Values \n" << G4endl;
      
      /// 
      G4cout << "MothPartID= " << Moth_Part_ID 
	     << "\t Part_Type= " << Part_Type 
	     << "\t  Moth_Part_Type= " << Moth_Part_Type
	     << "\t  Tot_Edep= " << Tot_Edep
	     << "\t  ZStart_Pos= " <<  ZStart_Pos
	     << "\t  Moth ZStart_Pos= " <<  Moth_ZStart_Pos
	     << "\t  ADDRESS of Moth ZStart_Pos= " <<  &Moth_ZStart_Pos
	     << G4endl;
      G4cout << "\n" << G4endl;
    }
  
}

TrackParentParticle::~TrackParentParticle()
{
  bool printvalues = false;
    
  if(printvalues)  G4cout << "Deleting TrackParentParticle Object" << G4endl;;
}


//////////////****************OLD*****************///////////////////////////
//PRESENT PARTICLE

/// G4double TrackParentParticle::TrackAddEDep(G4double e)
/// {
///   Tot_Edep+=e;
///   return Tot_Edep;
/// }
/// 
/// void TrackParentParticle::SetPart_Type(G4int ptype)
/// {
///   Part_Type = ptype;
///   G4cout << "Particle Type " << Part_Type << " set " << G4endl;
///   return;
/// }
/// 
/// void TrackParentParticle::SetMothPart_ID(G4int mpID)
/// {
///   Moth_Part_ID = mpID;
///   G4cout << "Mother Particle ID " << Moth_Part_ID << " set " << G4endl;
///   return;
/// }
/// 
/// void TrackParentParticle::SetEdep(G4double ed)
/// {
///   Tot_Edep = ed;
///   G4cout << "Particle Edep " << Tot_Edep << " set " << G4endl;
///   return;
/// }
/// 
/// void TrackParentParticle::SetPart_name(G4String pname)
/// {
///   Part_name = pname;
///   G4cout << "Particle Name " << Part_name << " set " << G4endl;
///   return;
/// }
/// 
/// void TrackParentParticle::SetZStart_Pos(G4double zstpos)
/// {
///   ZStart_Pos = zstpos;
///   G4cout << "Particle Z Start Position " << ZStart_Pos << " set " << G4endl;
///   return;
/// }
/// 
/// void TrackParentParticle::SetStart_Time(G4double stime)
/// {
///   Start_Time = stime;
///   G4cout << "Particle Start Time " << Start_Time << " set " << G4endl;
///   return;
/// }


//MOTHER PARTICLE

/// void TrackParentParticle::SetMoth_Part_Type(G4int pptype)
/// {
///   Moth_Part_Type = pptype;
///   G4cout << "Mother Particle Type " << Moth_Part_Type << " set " << G4endl;
///   return;
/// }
/// 
/// 
/// void TrackParentParticle::SetMoth_Edep(G4double pped)
/// {
///   Moth_Tot_Edep = pped;
///   G4cout << "Mother Particle Edep " << Moth_Tot_Edep << " set " << G4endl;
///   return;
/// }
/// 
/// void TrackParentParticle::SetMoth_Part_name(G4String ppname)
/// {
///   Moth_Part_name = ppname;
///   G4cout << "Mother Particle Name " << Moth_Part_name << " set " << G4endl;
///   return;
/// }
/// 
/// void TrackParentParticle::SetMoth_ZStart_Pos(G4double pzs)
/// {
///   Moth_ZStart_Pos = pzs;
///   G4cout << "Mother Particle Z Start Position " << Moth_ZStart_Pos << " set " << G4endl;
///   return;
/// }
/// 
/// void TrackParentParticle::SetMoth_Start_Time(G4double ppstime)
/// {
///   Moth_Start_Time = ppstime;
///   G4cout << "Mother Particle Start Time " << Moth_Start_Time << " set " << G4endl;
///   return;
/// }

////****************************************************************////////

void TrackParentParticle::SetIntValues(G4int* IntArray)
{
  Moth_Part_ID = IntArray[0];
  
  Part_Type = IntArray[1];
  

  Moth_Part_Type = IntArray[2];
  

  bool printvalues = false;

  if(printvalues)
    {
      G4cout << "Mother Particle ID " << Moth_Part_ID << " set " << G4endl;
      G4cout << "Particle Type " << Part_Type << " set " << G4endl;
      G4cout << "Mother Particle Type " << Moth_Part_Type << " set " << G4endl;
    }
  return;
}

void TrackParentParticle::SetDoubleValues(G4double* DoubleArray)
{
  Tot_Edep = DoubleArray[0];
  
  Moth_Tot_Edep = DoubleArray[1];
  
  ZStart_Pos = DoubleArray[2];
  
  Moth_ZStart_Pos = DoubleArray[3];
      
  Start_Time = DoubleArray[4];
  
  Moth_Start_Time = DoubleArray[5];
    
  bool printvalues = false;

  if(printvalues)
    {
      G4cout << "Particle Edep " << G4BestUnit(Tot_Edep,"Energy") 
	     << " set " << G4endl;
      G4cout << "Mother Particle Edep " << G4BestUnit(Moth_Tot_Edep,"Energy") 
	     << " set " << G4endl;
      G4cout << "Particle Z Start Position " << G4BestUnit(ZStart_Pos,"Length") 
	     << " set " << G4endl;
      G4cout << "Mother Particle Z Start Position " << G4BestUnit(Moth_ZStart_Pos,"Length") 
	     << " set " << G4endl;
      G4cout << "Particle Start Time " << G4BestUnit(Start_Time,"Time") 
	     << " set " << G4endl;
      G4cout << "Mother Particle Start Time " 
	     << G4BestUnit(Moth_Start_Time,"Time") << " set " << G4endl;
    }
    
  return;
}


//OPERATOR DEFINITION

/// TrackParentParticle TrackParentParticle::operator=(const TrackParentParticle &original)
/// {
///   this->Moth_Part_ID = original.Moth_Part_ID;
///   this->Part_Type = original.Part_Type;
///   this->Moth_Part_Type = original.Moth_Part_Type;
///   this->Tot_Edep = original.Tot_Edep;
///   this->ZStart_Pos = original.ZStart_Pos;
///   this->Moth_ZStart_Pos = original.Moth_ZStart_Pos;
///   
///   return *this;
/// }
///   
/// void* TrackParentParticle::operator new(std::size_t size)
/// {
///   assert (size == sizeof (TrackParentParticle));
///   return malloc(size);
/// 
///   
/// }

