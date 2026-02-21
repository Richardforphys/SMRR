#include "NeutronGEMDataManager.hh"
#include "NeutronGEMHistoManager.hh"

NeutronGEMDataManager::NeutronGEMDataManager() :
		fNumberOfEvents(0), fHistoManager(NULL) {
	fCounters = new G4int[NUM_COUNTERS];
}

NeutronGEMDataManager::~NeutronGEMDataManager() {
	delete fHistoManager;
	delete[] fCounters;
	G4cout << "Deconstructor NeutronGEMDataManager" << G4endl;
}

NeutronGEMDataManager* NeutronGEMDataManager::fDataManager = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

NeutronGEMDataManager* NeutronGEMDataManager::GetInstance() {
	if (!fDataManager) {
		fDataManager = new NeutronGEMDataManager();
	}
	return fDataManager;
}
void NeutronGEMDataManager::Dispose() {
	delete fDataManager;
	fDataManager = 0;
}

NeutronGEMHistoManager* NeutronGEMDataManager::getHistoManager() {
	return fHistoManager;
}

void NeutronGEMDataManager::setHistoManager(
		NeutronGEMHistoManager* histoManager) {
	fHistoManager = histoManager;
}

void NeutronGEMDataManager::setDescription(G4String description) {
	fDescription = description;
}

void NeutronGEMDataManager::setFilenameGDML(G4String filenameGDML) {
	fFilenameGDML = filenameGDML;
}

G4String NeutronGEMDataManager::getFilenameGDML() {
	return fFilenameGDML;
}

G4String NeutronGEMDataManager::getDescription() {
	return fDescription;
}

G4int NeutronGEMDataManager::getNumberOfEvents() {
	return fNumberOfEvents;
}

void NeutronGEMDataManager::setNumberOfEvents(G4int numberOfEvents) {
	fNumberOfEvents = numberOfEvents;
}

void NeutronGEMDataManager::setCathodeThickness(G4double thickness)
{
	fCathodeThickness = thickness;
}

void NeutronGEMDataManager::setConverterThickness(G4double thickness)
{
	fConverterThickness = thickness;
}

void NeutronGEMDataManager::setSEEThickness(G4double thickness)
{
	fSEEThickness = thickness;
}

void NeutronGEMDataManager::setDriftThickness(G4double thickness)
{
	fDriftThickness = thickness;
}

G4double NeutronGEMDataManager::getCathodeThickness()
{
	return fCathodeThickness;
}

G4double NeutronGEMDataManager::getConverterThickness()
{
	return fConverterThickness;
}

G4double NeutronGEMDataManager::getSEEThickness()
{
	return fSEEThickness;
}

G4double NeutronGEMDataManager::getDriftThickness()
{
	return fDriftThickness;
}

void NeutronGEMDataManager::setCathodeMaterial(G4String material)
{
	fCathodeMaterial = material;
}

void NeutronGEMDataManager::setConverterMaterial(G4String material)
{
	fConverterMaterial = material;
}

void NeutronGEMDataManager::setSEEMaterial(G4String material)
{
	fSEEMaterial = material;
}

G4String NeutronGEMDataManager::getCathodeMaterial()
{
	return fCathodeMaterial;
}

G4String NeutronGEMDataManager::getConverterMaterial()
{
	return fConverterMaterial;
}

G4String NeutronGEMDataManager::getSEEMaterial()
{
	return fSEEMaterial;
}

G4double NeutronGEMDataManager::getDriftField()
{
	return fDriftField;
}

void NeutronGEMDataManager::setDriftField(G4double driftField)
{
	fDriftField = driftField;
}

void NeutronGEMDataManager::resetCounters() {
	for(int i = 0; i < NUM_COUNTERS;i++)
	{
		fCounters[i] = 0;
	}

}

G4int* NeutronGEMDataManager::getCounters() {
	return fCounters;
}

void NeutronGEMDataManager::increaseCounter(int number) {
	 fCounters[number]++;
}
