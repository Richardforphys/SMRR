#include "GarfieldPhysics.hh"

#include "TGeoManager.h"
#include "/home/ubuntu/SMRR/ROOT/root_src/geom/gdml/inc/TGDMLParse.h"
#include "NeutronGEMDataManager.hh"
#include "NeutronGEMHistoManager.hh"
#include "TGeoBBox.h"

#include "G4SystemOfUnits.hh"

GarfieldPhysics::GarfieldPhysics(const std::string configName) :
		fName(configName) {
	fMapParticlesEnergy = new MapParticlesEnergy();
	fSecondaryElectrons = new std::vector<GarfieldElectron*>();
}

GarfieldPhysics::~GarfieldPhysics() {
	delete fMapParticlesEnergy;
	DeleteSecondaryElectrons();
	delete fSecondaryElectrons;
	delete fMediumMagboltz;
	delete fSensor;
	delete fAvalalancheMicroscopic;
	delete fComponentConstant;
	delete fTrackHeed;
	delete fTrackElectron;
	delete fGeometryRoot;

	G4cout << "Deconstructor GarfieldPhysics" << G4endl;
}

void GarfieldPhysics::AddParticleName(const std::string particleName,
		double ekin_min_keV, double ekin_max_keV) {
	if (ekin_min_keV >= ekin_max_keV) {
		std::cout << "Ekin_min=" << ekin_min_keV
				<< " keV is larger than Ekin_max=" << ekin_max_keV << " keV"
				<< std::endl;
		return;
	}
	std::cout << "Garfield model is applicable for G4Particle " << particleName
			<< " between " << ekin_min_keV << " keV and " << ekin_max_keV
			<< " keV" << std::endl;
	fMapParticlesEnergy->insert(
			std::make_pair(particleName,
					std::make_pair(ekin_min_keV, ekin_max_keV)));

}

bool GarfieldPhysics::FindParticleName(std::string name) {
	MapParticlesEnergy::iterator it;
	it = fMapParticlesEnergy->find(name);
	if (it != fMapParticlesEnergy->end()) {
		return true;
	}
	return false;
}

bool GarfieldPhysics::FindParticleNameEnergy(std::string name,
		double ekin_keV) {
	MapParticlesEnergy::iterator it;
	it = fMapParticlesEnergy->find(name);
	if (it != fMapParticlesEnergy->end()) {
		EnergyRange_keV range = it->second;
		if (range.first <= ekin_keV && range.second >= ekin_keV) {
			return true;
		}
	}
	return false;
}

void GarfieldPhysics::InitializePhysics() {

	fMediumMagboltz = new Garfield::MediumMagboltz();
	fSensor = new Garfield::Sensor();
	fAvalalancheMicroscopic = new Garfield::AvalancheMicroscopic();

	fMediumMagboltz->SetComposition("ar", 70., "co2", 30.);
	fMediumMagboltz->SetTemperature(293.15);
	fMediumMagboltz->SetPressure(760.);
	fMediumMagboltz->EnableDebugging();
	fMediumMagboltz->Initialise(true);
	fMediumMagboltz->DisableDebugging();

	// Set the Penning transfer efficiency.
	const double rPenning = 0.57;
	const double lambdaPenning = 0.;
	fMediumMagboltz->EnablePenningTransfer(rPenning, lambdaPenning, "ar");
	// Load the ion mobilities.
	fMediumMagboltz->LoadIonMobility(
			"/afs/cern.ch/user/d/dpfeiffe/garfield/Data/IonMobility_Ar+_Ar.txt");

	fComponentConstant = new Garfield::ComponentConstant();
	fComponentConstant->SetElectricField(0, 0, 1000);

	fTrackHeed = new Garfield::TrackHeed();
	fTrackHeed->SetSensor(fSensor);
	fTrackHeed->EnableDeltaElectronTransport();
	fTrackElectron = new Garfield::TrackElectron();
	fTrackElectron->SetSensor(fSensor);

	CreateGeometryFromGDML("drift.gdml", "ArCO2");

}

void GarfieldPhysics::CreateGeometryFromGDML(std::string geometryFile,
		std::string gasMedium) {
	fGeoManager = new TGeoManager("world", "geometry");
	TGDMLParse gdmlparser;
	TGeoVolume *top = new TGeoVolume();
	G4String file = "./" + geometryFile;
	top = gdmlparser.GDMLReadFile(file.c_str());
	TGeoBBox* box = (TGeoBBox*) top->GetShape();
	double dx, dy, dz;
	dx = box->GetDX();
	dy = box->GetDY();
	dz = box->GetDZ();
	fGeoManager->SetTopVolume(top);
	fGeoManager->CloseGeometry();

	TGeoMaterial* material = top->GetMaterial();
	material->Print();

	fGeometryRoot = new Garfield::GeometryRoot();
	fGeometryRoot->SetGeometry(fGeoManager);
	fGeometryRoot->SetMedium(gasMedium.c_str(), fMediumMagboltz);

	fSensor->SetArea(-dx, -dy, -dz, dx, dy, dz);
	G4cout << "\nArea of drift: " << "dx=" << dx << "cm, dy=" << dy << "cm, dz="
			<< dz << "cm" << G4endl;
	fAvalalancheMicroscopic->SetSensor(fSensor);
	fComponentConstant->SetGeometry(fGeometryRoot);
	fSensor->AddComponent(fComponentConstant);
}

void GarfieldPhysics::DoIt(std::string particleName, double ekin_keV,double time,
		double x_cm, double y_cm, double z_cm, double dx, double dy,
		double dz, bool createSecondaries) {


	DeleteSecondaryElectrons();

	NeutronGEMDataManager* dataManager = NeutronGEMDataManager::GetInstance();
	NeutronGEMHistoManager* histoManager = dataManager->getHistoManager();

	double eKin_eV = ekin_keV * 1000;

	double xc = 0., yc = 0., zc = 0., tc = 0.;
	// Number of electrons produced in a collision
	int nc = 0;
	// Energy loss in a collision
	double ec = 0.;
	// Dummy variable (not used at present)
	double extra = 0.;
	// Total energy loss along the track
	double esum = 0.;
	// Total number of electrons produced along the track
	int nsum = 0;

	if (particleName == "e-" || particleName == "electron") {
			fTrackHeed->SetParticle(particleName);
			fTrackHeed->SetKineticEnergy(eKin_eV);
			fTrackHeed->NewTrack(x_cm, y_cm, z_cm, time, dx, dy, dz);
			//fAvalalancheMicroscopic->AvalancheElectron(x_cm, y_cm, z_cm, time,eKin_eV, dx, dy, dz);
			xc = 0.;
			yc = 0;
			zc = 0.;
			tc = 0.;
			nc = 0;
			ec = 0.;
			esum = 0.;
			nsum = 0;
			//int n_e, n_i;
			//fAvalalancheMicroscopic->GetAvalancheSize(n_e, n_i);
			//std::cout << "number of electrons " << n_e << std::endl;

			while (fTrackHeed->GetCluster(xc, yc, zc, tc, nc, ec, extra)) {
				esum += ec;
				nsum += nc;

				for (int i = 0; i < nc; ++i) {
					double xe, ye, ze, te;
					double ee, dxe, dye, dze;
					fTrackHeed->GetElectron(i, xe, ye, ze, te, ee, dxe, dye,
							dze);
					if(createSecondaries)
					{
						fSecondaryElectrons->push_back(new GarfieldElectron(xe, ye, ze, ee, te, dxe, dye,dze));
					}
					histoManager->Fill3DEnergyElectrons(1,xe,ye,ze,ee);
					//G4cout << "       e-: x=" << xe << "cm, y=" << ye << "cm, z=" << ze << "cm, E=" << ee << "eV" << G4endl;
					dataManager->increaseCounter(11);
					histoManager->AddClustersConversionElectrons();


				}
				histoManager->fillHistogram(21,(double)(esum*0.001));

			}
		} else if (particleName == "gamma") {
		esum = 0.;
		fTrackHeed->TransportPhoton(x_cm, y_cm, z_cm, time, eKin_eV, dx, dy, dz,
				nsum);
		for (int i = 0; i < nsum; ++i) {
			double xe, ye, ze, te;
			double ee, dxe, dye, dze;
			fTrackHeed->GetElectron(i, xe, ye, ze, te, ee, dxe, dye, dze);
			//std::cout << "Ekin=" << ee << std::endl;
			if(createSecondaries)
			{
				fSecondaryElectrons->push_back(new GarfieldElectron(xe, ye, ze, ee, te,dxe, dye,dze));
			}
			histoManager->Fill3DEnergyElectrons(4,xe,ye,ze,ee);
			esum += ee;
			//G4cout << "       e-: x=" << xe << "cm, y=" << ye << "cm, z=" << ze << "cm, E=" << ee << "eV" << G4endl;
			dataManager->increaseCounter(14);
			histoManager->AddClustersGammas();
		}

		histoManager->fillHistogram(24, (double)(esum*0.001));

	}
	/*
	if (nsum > 0) {
		G4cout << "In drift: " << ekin_keV << " keV " << particleName << G4endl;
		G4cout << ekin_keV << " keV " << particleName << G4endl;
		G4cout << "new track: x" << x_cm << " y" << y_cm << " z" << z_cm << " dx" << dx << " dy" << dy << " dz" << dz << G4endl;
		G4cout << "number of secondary electrons: " << nsum << ", E=" << esum << "eV \n" << G4endl;
	}
	*/
}


std::vector<GarfieldElectron*>* GarfieldPhysics::GetSecondaryElectrons()
{
	return fSecondaryElectrons;
}


void GarfieldPhysics::DeleteSecondaryElectrons()
{
	if(! fSecondaryElectrons->empty())
	{
		fSecondaryElectrons->erase(fSecondaryElectrons->begin(), fSecondaryElectrons->end());
	}
}

