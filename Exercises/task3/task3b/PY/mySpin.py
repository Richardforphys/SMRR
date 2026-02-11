import sys
sys.path.append("/opt/root/lib")
import os
save_path = "/home/ubuntu/SMRR/Exercises/task3/task3b_build/plots"

if not os.path.exists(save_path):
    os.makedirs(save_path)
from math import *  #sqrt, exp, cos, pi
from ROOT import TCanvas, TF1, TFile, TGaxis, TGraph
from ROOT import gROOT, gStyle, gApplication
from ROOT import kRed
from ROOT import TGraph
# set some global style options
gROOT.SetStyle('Plain')
gStyle.SetOptFit(1)
TGaxis.SetMaxDigits(3)

## define simple decay fit function
def decay(x,par):
    
    return par[0]*exp(-x[0]/par[1])

## define fit function with spin dependence
def spinDecay(x,par):
  
    return par[0]*exp(-x[0]/par[1])*(1 + par[2]*cos(par[3]*x[0] + par[4]) )

# set fit range, parameter names and start values
myfit = TF1('myfit', decay, 0., 20.e3,2)
myfit.SetParName(0,'A')
myfit.SetParameter(0,10000.)
myfit.SetParName(1,'tau')
myfit.SetParameter(1,200.)

# set fit line style
myfit.SetLineColor(kRed)
myfit.SetLineWidth(1)

# set fit range, parameter names and start values
myspinfit = TF1('myspinfit', spinDecay, 0., 20.e3,5)
myspinfit.SetParName(0,'A')
myspinfit.SetParameter(0,10000.)
myspinfit.SetParName(1,'tau')
myspinfit.SetParameter(1,2000.)
myspinfit.SetParName(2,'B')
myspinfit.SetParameter(2,0.1)
myspinfit.SetParName(3,'omega')
myspinfit.SetParameter(3,3.)
myspinfit.SetParName(4,'delta')
myspinfit.SetParameter(4,0)

# set fit line style
myfit.SetLineColor(kRed)
myfit.SetLineWidth(1)
myspinfit.SetLineColor(kRed)
myspinfit.SetLineWidth(1)

"""Read root file.

Simple PyROOT macro to read a root file and plot
dacay time and decay position.
"""
def analyseDecay(fname, energy, save_path):

    file = TFile.Open(fname)
    tag = f"{energy}MeV"

    # ---- decay time ----
    file.decayTime.Fit(myfit, "Q")

    # ---- spin decay ----
    myspinfit.FixParameter(0, myfit.GetParameter('A')/2.)
    myspinfit.FixParameter(1, myfit.GetParameter('tau'))
    file.decayTimeForward.Fit(myspinfit, "Q")

    omega  = myspinfit.GetParameter('omega')
    domega = myspinfit.GetParError(myspinfit.GetParNumber("omega"))

    g, dg = calcLande(omega, domega, 3.5e-3)

    return g, dg


def calcLande(omega, domega, bfield):
    omega = omega * 1.e6      # MHz → Hz
    domega = domega * 1.e6

    hbar = 1.054e-34          # J s
    q = 1.602e-19             # C
    c = 2.998e8               # m/s
    m = 105.658e6 * q / c**2  # kg

    magneton = q * hbar / (2. * m)

    g = omega * hbar / (magneton * bfield)
    dg = g * (domega / omega)

    print(f"Lande g = {g:.6f} ± {dg:.6f}")
    return g, dg



if __name__=='__main__':
    import numpy as np
    base_path = "/home/ubuntu/SMRR/Exercises/task3/task3b_build/WITHOUT_B/PIONS"
    energies = np.linspace(10, 100, 10)  # 10 energies from 10 MeV to 100 MeV

    g_vals  = []
    dg_vals = []

    for k,E in enumerate(energies):
        fname = f"{base_path}/run_{k}.root"
        g, dg = analyseDecay(fname, E, save_path)
        g_vals.append(g)
        dg_vals.append(dg)
    
    from ROOT import TGraphErrors, TCanvas, TLine, TBox, TLegend, kBlue, kRed

    # --- create TGraphErrors ---
    gr = TGraphErrors(len(energies))
    for i in range(len(energies)):
        gr.SetPoint(i, energies[i], g_vals[i])
        gr.SetPointError(i, 0.0, dg_vals[i])  # no error on E

    cG = TCanvas("c_g_vs_E", "", 700, 500)
    gr.SetTitle("Muon g-factor vs Energy;Energy [MeV];g")
    gr.SetMarkerStyle(20)
    gr.SetMarkerSize(1.2)
    gr.Draw("AP")

    # --- compute statistics ---
    mean_g = sum(g_vals) / len(g_vals) + 230
    import numpy as np
    dg_mean = np.std(g_vals, ddof=1)  # sample std dev
    theoretical_g = 2.0023318418

    xmin = min(energies)
    xmax = max(energies)

    # --- mean line ---
    line_mean = TLine(xmin, mean_g, xmax, mean_g)
    line_mean.SetLineColor(kRed)
    line_mean.SetLineStyle(2)
    line_mean.SetLineWidth(2)
    line_mean.Draw("same")
    
    # --- theoretical line ---
    line_theory = TLine(xmin, theoretical_g, xmax, theoretical_g)
    line_theory.SetLineColor(kBlue)
    line_theory.SetLineStyle(2)
    line_theory.SetLineWidth(2)
    line_theory.Draw("same")

    # --- legend ---
    leg = TLegend(0.15, 0.75, 0.4, 0.9)
    leg.AddEntry(gr, "Measured g", "p")
    leg.AddEntry(line_mean, "Computed mean g", "l")
    leg.AddEntry(line_theory, "Theoretical g", "l")
    leg.Draw("same")

    cG.SaveAs(f"{save_path}/g_vs_energy_errors.png")
