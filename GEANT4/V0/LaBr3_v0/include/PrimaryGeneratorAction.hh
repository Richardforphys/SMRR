#include "G4VUserPrimaryGeneratorAction.hh"
#include <fstream>
#include "G4SystemOfUnits.hh"

class G4VPrimaryGenerator;
class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
//This means that I am deriving the user defined "PrimaryGeneratorAction" class from a public class called "G4VUserPrimaryGeneratorAction"
    public:
        //Constructor
        PrimaryGeneratorAction();

        //Destructor
        ~PrimaryGeneratorAction();

        //Mandatory method to define primary particles
        void GeneratePrimaries(G4Event*);

    private:
        G4VPrimaryGenerator* InitializeGPS(); //I am going to use GenericParticleSource as generator
        G4VPrimaryGenerator* gun;
        std::ofstream* outfile; //FIle in which ther will be the particles list
};