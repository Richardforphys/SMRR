#include "SiDigi.hh"

G4Allocator<SiDigi> SiDigiAllocator;

SiDigi::SiDigi(const int& pn,const int& sn) :
		charge(0) ,
		stripNumber(sn) ,
		planeNumber(pn) ,
		isPrimary(false)
{

}

void SiDigi::Print()
{
	G4cout<<"Plane: "<<planeNumber<<" Strip: "<<stripNumber<<" Charge: "<<charge<<" Primary: "<<isPrimary<<G4endl;
}
