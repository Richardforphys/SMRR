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
  G4cout<<"Digit: Plane= "<<planeNumber<<" Strip= "<<stripNumber<<" with Charge= "<<charge<<G4endl;
}
