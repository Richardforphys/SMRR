
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <vector>
#include <TCanvas.h>
#include <TROOT.h>
#include <TApplication.h>
#include <TH1F.h>
#include <TF1.h>
#include <TGeoManager.h>
#include <TGeoMaterial.h>
#include <TGeoMedium.h>
#include <TGeoVolume.h>
#include <TGeoBBox.h>
#include <TGeoTube.h>
#include <TGeoPcon.h>
#include <TGeoHalfSpace.h>
#include <TGeoMatrix.h>
#include <TGeoCompositeShape.h>
#include <TMath.h>
#include <TFile.h>
#include <TTree.h>
#include <TNtuple.h>

#include "Garfield/ComponentAnsys123.hh"
#include "Garfield/ViewField.hh"
#include "Garfield/MediumMagboltz.hh"
#include "Garfield/Sensor.hh"
#include "Garfield/AvalancheMicroscopic.hh"
#include "Garfield/AvalancheMC.hh"
#include "Garfield/Random.hh"

#include "Garfield/TrackHeed.hh"
#include "Garfield/SolidTube.hh"
#include "Garfield/GeometrySimple.hh"
#include "Garfield/ComponentConstant.hh"
#include "Garfield/Sensor.hh"
#include "Garfield/FundamentalConstants.hh"
#include "Garfield/SolidBox.hh"


using namespace Garfield;


class Data_GEM
{
public:
    double xe;
    double ye;
    double ze;
    int ii;
    int rx_number;
    Data_GEM()
    {
        xe=0;
        ye=0;
        ze=0;
        ii=0;
        rx_number=0;
    }
};

void create_txt_singolo(double xe,double ye,double ze,int ii,int rx_number)
{
    //SCRIVO IL FILE TXT
    FILE * pFile;
    std::string title = "output.txt";
    const char * path = title.c_str();
    pFile = fopen (path,"a");
    fprintf(pFile, "%d\t%d\t%f\t%f\t%f\n",rx_number,ii,xe,ye,ze);
    fclose(pFile);
}

void create_txt_vector(std::vector<Data_GEM> &data_gem_arr, double energy)
{
    long long limit = data_gem_arr.size();

    // Costruzione nome file con energia
    std::ostringstream filename;
    filename << "output_E_" << std::fixed << std::setprecision(2) << energy << ".txt";
    std::string title = filename.str();

    FILE *pFile = fopen(title.c_str(), "w");

    if (!pFile) {
        perror("Errore apertura file");
        return;
    }

    for(long long ll = 0; ll < limit; ll++)
    {
        fprintf(pFile, "%d\t%d\t%f\t%f\t%f\n",
                data_gem_arr[ll].rx_number,
                data_gem_arr[ll].ii,
                data_gem_arr[ll].xe,
                data_gem_arr[ll].ye,
                data_gem_arr[ll].ze);
    }

    fclose(pFile);
}


void write_fit_results(double egamma,
                       double ampiezza,
                       double centro,
                       double centro_err,
                       double sigma,
                       double sigma_err,
                       double fwhm,
                       double eff,
                       double fwhm_pois)
{
    std::ofstream file("fit_results_XeSF6.txt", std::ios::app); 

    file.seekp(0, std::ios::end);
    if (file.tellp() == 0) {
        file << std::setw(10) << "Egamma(eV)"
             << std::setw(15) << "Ampiezza"
             << std::setw(15) << "Centro"
             << std::setw(15) << "Err_Centro"
             << std::setw(15) << "Sigma"
             << std::setw(15) << "Err_Sigma"
             << std::setw(15) << "FWHM(%)"
             << std::setw(15) << "Eff(%)"
             << std::setw(15) << "FWHM_Pois(%)"
             << std::endl;
    }

    file << std::setw(10) << egamma
         << std::setw(15) << ampiezza
         << std::setw(15) << centro
         << std::setw(15) << centro_err
         << std::setw(15) << sigma
         << std::setw(15) << sigma_err
         << std::setw(15) << fwhm
         << std::setw(15) << eff
         << std::setw(15) << fwhm_pois
         << std::endl;

    file.close();
}

int main(int argc, char * argv[])
{

    //Construct the GEM
    //Questi sono i parametri di ANSYS. Se cambi modello in ANSYS cambia i parametri anche qui
    double pitch =  0.140*0.1;   // Distance between holes, in mm
    double kapton = 0.050*0.1;   //Thickness of the kapton layer, in mm
    double metal =  0.005*0.1;   //Thickness of the meta layers, in mm
    double outdia = 0.070*0.1;   //Hole outer diameter, in mm
    double middia = 0.050*0.1;   //Hole diameter in the centre, in mm
    double drift =   3.0*0.1;    //Thickness of the drift region
    double induct = 1.0*0.1;    //Thickness of the induction gap
    double rim =    0.074*0.1;   //Rim diameter, in mm
    double t1=1;
    double t2=2;
    double n_gem=3; //number of gem foil

    //Calcolo le coordinate in z di anodo,catoro e prima gem
    double cathode_z=-drift;
    double anode_z=(t1+kapton+(2*metal))+(t2+kapton+(2*metal))+induct;
    double first_gem_z=-kapton-metal;

    std::cout<<"Cathode coordinate z"<<cathode_z<<std::endl;
    std::cout<<"First GEM coordinate z"<<first_gem_z<<std::endl;
    // Make a gas medium.
    MediumMagboltz* gas = new MediumMagboltz();
    gas->SetComposition("Xe", 70., "SF6", 30.);
    gas->SetTemperature(293.15);
    gas->SetPressure(AtmosphericPressure);

    // Gas gap [cm].
    const double width = 10.;
    SolidBox* box = new SolidBox(0., 0., (cathode_z+first_gem_z)/2., width / 2., width/2., (first_gem_z-cathode_z)/2.);
    GeometrySimple* geo = new GeometrySimple();
    geo->AddSolid(box, gas);


    // Make a component with constant electric field.
    ComponentConstant field;
    field.SetGeometry(geo);
    field.SetElectricField(0., 0., 300.);

    // Make a sensor.
    Sensor sensor;
    sensor.AddComponent(&field);

    // Use Heed for simulating the photon absorption.
    TrackHeed track;
    track.SetSensor(&sensor);
    track.EnableElectricField();
    TFile *f = new TFile("ntuples.root", "RECREATE");
    //TNtuple* ntuple  = new TNtuple("ntuple","","xe1:ye1:ze1:te1:e1:xe2:ye2:ze2:te2:e2");
    TNtuple* ntuple  = new TNtuple("ntuple","","rxn:epn:xe1:ye1:ze1:dxe1:dye1:dze1");

    // Histogram
    //TH1F hXe("hElectrons", "Number of electrons", nBins, -0.008, 0.036*3);;
    const int nEvents = 50000;
    std::vector<Data_GEM> data_gem_arr;
    int stop=0;
    std::cout<<"I fotoni partono da z= "<<cathode_z+0.01<<std::endl;
    double egamma = argv[1] ? atof(argv[1]) : 3500.;
    TH1::StatOverflows(true);
    int nBins = 1000;
    std::string histoname = "hElectrons_" + std::to_string(static_cast<int>(egamma)) + "eV";
    std::string histonameDrift = "hElectronsDrift_" + std::to_string(static_cast<int>(egamma)) + "eV";
    TH1F hElectrons(histoname.c_str(),
                    "Number of electrons;N_{e};Entries",
                    nBins,
                    -0.5,
                    1000-0.5);
    for (unsigned int i = 0; i < nEvents; ++i)
    {
        //if (i % 1000 == 0) std::cout << i << "/" << nEvents << "\n";
        // Initial coordinates of the photon.
        const double x0 = 0.;
        const double y0 = 0.;
        const double z0 = cathode_z+0.01;
        const double t0 = 0.;
        // Sample the photon energy, using the relative intensities according to XDB.
        const double r = 1. * RndmUniform();
        int ne = 0;
        track.TransportPhoton(x0, y0, z0, t0, egamma, 0., 0., 1., ne);

        if(ne>0)
        {
            for(int ii=0; ii<ne; ii++)
            {
                double xe;
                double ye;
                double ze;
                double te;
                double ee;
                double dxe;
                double dye;
                double dze;
                Data_GEM data_gem_act;
                track.GetElectron(ii,xe,ye,ze,te,ee,dxe,dye,dze);
                data_gem_act.ii=ii;
                data_gem_act.rx_number=i;
                data_gem_act.xe=xe;
                data_gem_act.ye=ye;
                data_gem_act.ze=ze;
                data_gem_arr.push_back(data_gem_act);
            }
            hElectrons.Fill(ne);
        }

        //bool GetElectron(const int i, double& x, double& y, double& z, double& t,
        //double& e, double& dx, double& dy, double& dz);
    }
    f->Write();

    create_txt_vector(data_gem_arr, egamma);
    
    double center_guess = hElectrons.GetMaximumBin();
    TF1 fitfunc("fitfunc","gaus");
    TCanvas c("c", "", 600, 600);
    c.cd();
    hElectrons.SetLineColor(kBlack);
    hElectrons.SetTitle("Number of electrons;N_{e};Entries");
    hElectrons.SetLineWidth(2);
    hElectrons.SetFillColorAlpha(kGray,0.5);
    hElectrons.SetFillStyle(3005);
    hElectrons.GetXaxis()->SetRangeUser(0, center_guess+1000);
    hElectrons.Draw();
    hElectrons.Fit("fitfunc","","",center_guess-100,center_guess+100);

    std::string folder = "/home/ubuntu/SMRR/garfield_tasks/Rx_build/PNGS_Ar/";
    std::string filename = folder + "hElectrons_" +
                        std::to_string(static_cast<int>(egamma)) +
                        "eV.png";

    c.SaveAs(filename.c_str());    
    double ampiezza=fitfunc.GetParameter(0);
    double centro=fitfunc.GetParameter(1);
    double sigma=fitfunc.GetParameter(2);
    double sigma_err=fitfunc.GetParError(2);
    double centro_err=fitfunc.GetParError(1);
    double fwhm=((sigma*2.35)/centro)*100;
    double eff=(hElectrons.GetEntries()/nEvents)*100;
    double fwhm_pois=(1/TMath::Sqrt(centro))*100;

    write_fit_results(egamma,
                  ampiezza,
                  centro,
                  centro_err,
                  sigma,
                  sigma_err,
                  fwhm,
                  eff,
                  fwhm_pois);
                  


    std::cout << "*****************************************************" << std::endl;
    std::cout << "Center = " << centro<< " FWHM = "<<fwhm<<"%"<<" EFF = "<<eff<<"%" <<" FWHM poisson = " <<fwhm_pois<<"%"<< std::endl;
    std::cout << "*****************************************************" << std::endl;
}