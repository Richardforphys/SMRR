
#include <iostream>
#include <fstream>
#include <cmath>

#include <TCanvas.h>
#include <TROOT.h>
#include <TApplication.h>
#include <TH1F.h>

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

#include "ComponentAnsys123.hh"
#include "ViewField.hh"
#include "MediumMagboltz.hh"
#include "Sensor.hh"
#include "AvalancheMicroscopic.hh"
#include "AvalancheMC.hh"
#include "Random.hh"
#include "Plotting.hh"

#include "TrackHeed.hh"
#include "SolidTube.hh"
#include "GeometrySimple.hh"
#include "ComponentConstant.hh"
#include "Sensor.hh"
#include "FundamentalConstants.hh"
#include "SolidBox.hh"


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

void create_txt_vector(std::vector<Data_GEM> &data_gem_arr)
{
    //SCRIVO IL FILE TXT
    long long limit=data_gem_arr.size();
    FILE * pFile;
    std::string title = "output.txt";
    const char * path = title.c_str();
    pFile = fopen (path,"w");
    for(long long ll=0; ll<limit-1; ll++)
    {
        fprintf(pFile, "%d\t%d\t%f\t%f\t%f\n",data_gem_arr[ll].rx_number,data_gem_arr[ll].ii,data_gem_arr[ll].xe,data_gem_arr[ll].ye,data_gem_arr[ll].ze);
    }
    fclose(pFile);
}






int main(int argc, char * argv[])
{

    TApplication app("app", &argc, argv);
    plottingEngine.SetDefaultStyle();

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
    gas->SetComposition("Kr", 70., "CO2", 30.);
    //gas->SetComposition("Kr", 100.);
    //gas->SetComposition("Xe", 100.);//, "CO2", 30.);
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
    const int nBins = 1000;
    TH1::StatOverflows(true);
    TH1F hElectrons("hElectrons", "Number of electrons", nBins, -0.5, nBins - 0.5);
    TH1F hXe("hElectrons", "Number of electrons", nBins, -0.008, 0.036*3);;
    const int nEvents = 10000;
    std::vector<Data_GEM> data_gem_arr;
    int stop=0;
    std::cout<<"I fotoni partono da z= "<<cathode_z+0.01<<std::endl;
    for (unsigned int i = 0; i < nEvents; ++i)
    {
        if (i % 1000 == 0) std::cout << i << "/" << nEvents << "\n";
        // Initial coordinates of the photon.
        const double x0 = 0.;
        const double y0 = 0.;
        const double z0 = cathode_z+0.01;
        const double t0 = 0.;
        // Sample the photon energy, using the relative intensities according to XDB.
        const double r = 1. * RndmUniform();

        double egamma = 6000.;
        //if(r< 0.5)
        //{
        //    egamma=4500;
        //}
        //else
        //{
         //   egamma=6000;
        //}
        //const double egamma = r < 100. ? 5898.8 : r < 150. ? 5887.6 : 6490.4;
        //double egamma=10000.;
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
                ntuple->Fill(data_gem_act.rx_number,data_gem_act.ii,data_gem_act.xe,data_gem_act.ye,data_gem_act.ze,dxe,dye,dze);


            }
            hElectrons.Fill(ne);

        }

        //bool GetElectron(const int i, double& x, double& y, double& z, double& t,
        //double& e, double& dx, double& dy, double& dz);




    }
    f->Write();

    create_txt_vector(data_gem_arr);

    TF1 fitfunc("fitfunc","gaus");
    int center_guess= hElectrons.GetMaximumBin();
    TCanvas c("c", "", 600, 600);
    c.cd();
    hElectrons.SetFillColor(kRed);
    hElectrons.SetLineColor(kRed);
    hElectrons.GetXaxis()->SetRangeUser(0,center_guess+1000);
    hElectrons.Draw();
    hElectrons.Fit("fitfunc","","",center_guess-100,center_guess+100);

    //double x_max = hElectrons.GetXaxis()->GetBinCenter(binmax);
    double centro=fitfunc.GetParameter(1);
    double sigma=fitfunc.GetParameter(2);
    double fwhm=((sigma*2.35)/centro)*100;
    double eff=(hElectrons.GetEntries()/nEvents)*100;
    double fwhm_pois=(1/TMath::Sqrt(centro))*100;
    std::cout << "*****************************************************" << std::endl;
    std::cout << "Center = " << centro<< " FWHM = "<<fwhm<<"%"<<" EFF = "<<eff<<"%" <<" FWHM poisson = " <<fwhm_pois<<"%"<< std::endl;
    std::cout << "*****************************************************" << std::endl;
    //hElectrons.SetFillColor(kBlue + 2);
    //hElectrons.SetLineColor(kBlue + 2);
    //hXe.Draw();
    app.Run(true);
}
