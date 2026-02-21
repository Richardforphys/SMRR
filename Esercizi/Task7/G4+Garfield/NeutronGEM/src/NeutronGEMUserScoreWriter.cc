//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//

#include "NeutronGEMUserScoreWriter.hh"
#include "NeutronGEMDataManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4SDParticleFilter.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4VScoringMesh.hh"

#include <map>
#include <fstream>

#include "TH1D.h"
#include "TH2D.h"
#include "TH3D.h"
#include "TFile.h"
#include "TTree.h"

NeutronGEMUserScoreWriter::NeutronGEMUserScoreWriter(NeutronGEMDataManager* dataManager)
	: G4VScoreWriter(), fDataManager(dataManager) {
		;
}

NeutronGEMUserScoreWriter::~NeutronGEMUserScoreWriter() {
	;
}

void NeutronGEMUserScoreWriter::DumpQuantityToFile(const G4String & psName, const G4String & fileName,  const G4String & option) {
	//
	if(verboseLevel > 0) {
		G4cout << "User-defined DumpQuantityToFile() method is invoked."
			<< G4endl;
		G4cout << "  -- to obtain a projection of the quantity <"
			<< psName
			<< "> onto the x-y plane --" << G4endl;
	}

	// change the option string into lowercase to the case-insensitive.
	G4String opt = option;
	std::transform(opt.begin(), opt.end(), opt.begin(), (int (*)(int))(tolower));

	// confirm the option
	if(opt.size() == 0) opt = "";

	time_t rawtime;
	struct tm * timeinfo;
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );

	strftime (fStartTime,50,"%Y-%m-%d-%H-%M-%S",timeinfo);

	G4String unitName = fScoringMesh->GetPSUnit(psName);
	G4double unitValue = fScoringMesh->GetPSUnitValue(psName);
	strftime (fStartTime,50,"%Y-%m-%d-%H-%M-%S",timeinfo);
	G4int num = fDataManager->getNumberOfEvents();
	std::ostringstream s;
	s << num;

	G4String theFileName = fDataManager->getDescription() + "_" + s.str() + "_" + fScoringMesh->GetWorldName() + "_" + psName + "_" + unitName + "_" + fStartTime + ".root";


	TFile* rootFile = new TFile(theFileName,"RECREATE");
	if(!rootFile) {
		G4cout << " HistoManager::book :" 
			<< " problem creating the ROOT TFile "
			<< G4endl;
		return;
	}
	TH2D* histo2D[4]; 
	TH3D* histo3D;


	G4ThreeVector v = fScoringMesh->GetSize();
	G4int sizeX = v.getX();
	G4int sizeY = v.getY();
	G4int sizeZ = v.getZ();
	if(opt == "cm")
	{
		sizeX = v.getX()/10;
		sizeY = v.getY()/10;
		sizeZ = v.getZ()/10;
	}
	else if (opt == "m")
	{
		sizeX = v.getX()/100;
		sizeY = v.getY()/100;
		sizeZ = v.getZ()/100;
	}
	G4int scaleX = 2*sizeX/fNMeshSegments[0];
	G4int scaleY = 2*sizeY/fNMeshSegments[1];
	G4int scaleZ = 2*sizeZ/fNMeshSegments[2];

	auto fSMap = fScoringMesh->GetScoreMap();

	auto msMapItr = fSMap.find(psName);
	if(msMapItr == fSMap.end()) {
		G4cerr << "ERROR : DumpToFile : Unknown quantity, \""
			<< psName << "\"." << G4endl;
		return;
	}

	auto score = msMapItr->second->GetMap();

	histo3D = new TH3D(psName, msMapItr->first, fNMeshSegments[0], -sizeX, sizeX, fNMeshSegments[1], -sizeY, sizeY,fNMeshSegments[2],-sizeZ, sizeZ);
	if (!histo3D) G4cout << "\n can't create histo3D" << G4endl;
	for(int x = 0; x < fNMeshSegments[0]; x++) {
		for(int y = 0; y < fNMeshSegments[1]; y++) {
			for(int z = 0; z < fNMeshSegments[2]; z++) {
				G4int idx = GetIndex(x, y, z);
				auto value = score->find(idx);
				if(value != score->end()) 
				{
					double newVal = value->second->mean();
					histo3D->Fill(-sizeX+x*scaleX,-sizeY + y*scaleY,-sizeZ + z*scaleZ,newVal/unitValue);
				}
			}
		} 
	} 

	histo2D[1] = new TH2D("projection_xy",msMapItr->first, fNMeshSegments[0], -sizeX, sizeX, fNMeshSegments[1], -sizeY, sizeY);
	if (!histo2D[1]) G4cout << "\n can't create histo2D" << G4endl;
	for(int x = 0; x < fNMeshSegments[0]; x++) {
		for(int y = 0; y < fNMeshSegments[1]; y++) {
			for(int z = 0; z < fNMeshSegments[2]; z++) {
				G4int idx = GetIndex(x, y, z);
				auto value = score->find(idx);
				if(value != score->end()) 
				{
					double newVal = value->second->mean();
					histo2D[1]->Fill(-sizeX+x*scaleX,-sizeY + y*scaleY,newVal/unitValue);
				}
			}
		} 
	} 
	G4String xTitle = "x [" + opt + "]";
	histo2D[1]->SetXTitle(xTitle);
	histo2D[1]->GetXaxis()->CenterTitle();
	G4String yTitle = "y [" + opt + "]";
	histo2D[1]->SetYTitle(yTitle);
	histo2D[1]->GetYaxis()->CenterTitle();

	histo2D[2] = new TH2D("projection_zy",msMapItr->first, fNMeshSegments[2], -sizeZ, sizeZ, fNMeshSegments[1], -sizeY, sizeY);
	if (!histo2D[2]) G4cout << "\n can't create histo2D" << G4endl;
	for(int y = 0; y < fNMeshSegments[1]; y++) {
		for(int z = 0; z < fNMeshSegments[2]; z++) {
			for(int x = 0; x < fNMeshSegments[0]; x++) {
				G4int idx = GetIndex(x, y, z);
				auto value = score->find(idx);
				if(value != score->end()) 
				{
					double newVal = value->second->mean();
					histo2D[2]->Fill(-sizeZ + z*scaleZ,-sizeY + y*scaleY,newVal/unitValue);
				}
			}
		} 
	} 
	xTitle = "z [" + opt + "]";
	histo2D[2]->SetXTitle(xTitle);
	histo2D[2]->GetXaxis()->CenterTitle();
	yTitle = "y [" + opt + "]";
	histo2D[2]->SetYTitle(yTitle);
	histo2D[2]->GetYaxis()->CenterTitle();

	histo2D[3] = new TH2D("projection_zx",msMapItr->first, fNMeshSegments[2],  -sizeZ, sizeZ, fNMeshSegments[0],-sizeX, sizeX);
	if (!histo2D[3]) G4cout << "\n can't create histo2D" << G4endl;
	for(int z = 0; z < fNMeshSegments[2]; z++) {
		for(int x = 0; x < fNMeshSegments[0]; x++) {
			for(int y = 0; y < fNMeshSegments[1]; y++) {
				G4int idx = GetIndex(x, y, z);
				auto value = score->find(idx);
				if(value != score->end()) 
				{
					double newVal = value->second->mean();
					histo2D[3]->Fill(-sizeZ + z*scaleZ,-sizeX + x*scaleX,newVal/unitValue);
				}
			}
		} 
	} 
	xTitle = "z [" + opt + "]";
	histo2D[3]->SetXTitle(xTitle);
	histo2D[3]->GetXaxis()->CenterTitle();
	yTitle = "x [" + opt + "]";
	histo2D[3]->SetYTitle(yTitle);
	histo2D[3]->GetYaxis()->CenterTitle();

	if (rootFile) {
		rootFile->Write();   
		rootFile->Close();
		G4cout << "\n----> Score File is saved \n" << G4endl;
	}


}


