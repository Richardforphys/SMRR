/*
 * GarfieldPhysics.hh
 *
 *  Created on: Jul 17, 2014
 *      Author: dpfeiffe
 */

#ifndef GARFIELDMODELCONFIG_HH_
#define GARFIELDMODELCONFIG_HH_

#include <map>
#include <vector>
#include <iostream>

#include "Sensor.hh"
#include "AvalancheMicroscopic.hh"
#include "ComponentConstant.hh"
#include "TrackHeed.hh"
#include "TrackElectron.hh"
#include "MediumMagboltz.hh"
#include "GeometryRoot.hh"

typedef std::pair<double, double> EnergyRange_keV;
typedef std::map< const std::string, EnergyRange_keV> MapParticlesEnergy;

class GarfieldElectron{
public:
	GarfieldElectron( double ekin_eV,double time, double x_cm,double y_cm,double z_cm, double dx,double dy,double dz):fEkin_MeV(ekin_eV/1000000), fTime(time), fx_mm(10*x_cm),fy_mm(10*y_cm), fz_mm(10*z_cm),  fdx(dx), fdy(dy), fdz(dz){}
	~GarfieldElectron(){};

	double getX_mm() {return fx_mm;}
	double getY_mm(){return fy_mm;}
	double getZ_mm(){return fz_mm;}
	double getEkin_MeV(){return fEkin_MeV;}
	double getTime(){return fTime;}
	double getDX(){return fdx;}
	double getDY(){return fdy;}
	double getDZ(){return fdz;}


private:
	double fEkin_MeV, fTime, fx_mm,fy_mm,fz_mm,fdx,fdy,fdz;

};


class GarfieldPhysics {
public:

	GarfieldPhysics(const std::string);
	~GarfieldPhysics();


	void InitializePhysics();

	void CreateGeometryFromGDML(std::string geometryFile, std::string gasMedium);

	void DoIt(std::string particleName, double ekin_keV,double time,
			double x_cm, double y_cm, double z_cm, double dx, double dy, double dz, bool createSecondaries);

	void AddParticleName(const std::string particleName, double ekin_min_keV, double ekin_max_keV);
	bool FindParticleName(const std::string name);
	bool FindParticleNameEnergy(std::string name, double ekin_keV);
	std::vector<GarfieldElectron*>* GetSecondaryElectrons();
	void DeleteSecondaryElectrons();
private:
	MapParticlesEnergy* fMapParticlesEnergy;
	TGeoManager* fGeoManager;
	std::string fName;
	Garfield::MediumMagboltz* fMediumMagboltz;
	Garfield::Sensor* fSensor;
	Garfield::AvalancheMicroscopic* fAvalalancheMicroscopic;
	Garfield::ComponentConstant* fComponentConstant;
	Garfield::TrackHeed* fTrackHeed;
	Garfield::TrackElectron* fTrackElectron;
	Garfield::GeometryRoot* fGeometryRoot;
	std::vector<GarfieldElectron*>* fSecondaryElectrons;


};
#endif /* GARFIELDMODELCONFIG_HH_ */
