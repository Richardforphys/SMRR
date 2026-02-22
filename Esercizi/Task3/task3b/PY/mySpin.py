import sys
sys.path.append("/opt/root/lib")
import os
save_path = "/home/ubuntu/SMRR/Esercizi/Task3/task3b/Results_PbWO4/Electrons_Spin_PNG"

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
    c_decay = TCanvas(f"c_decay_{tag}", "", 700, 500)
    file.decayTime.Fit(myfit, "Q")
    file.decayTime.Draw("E")
    myfit.Draw("same")
    c_decay.SaveAs(os.path.join(save_path, f"decayTime_{tag}.png"))
    c_decay.Close()

    # ---- spin decay ----
    myspinfit.FixParameter(0, myfit.GetParameter('A')/2.)
    myspinfit.FixParameter(1, myfit.GetParameter('tau'))

    c_spin = TCanvas(f"c_spin_{tag}", "", 700, 500)
    file.decayTimeForward.Fit(myspinfit, "Q")
    file.decayTimeForward.Draw("E")
    myspinfit.Draw("same")
    c_spin.SaveAs(os.path.join(save_path, f"decayTimeForward_{tag}.png"))
    c_spin.Close()

    omega  = myspinfit.GetParameter('omega')
    domega = myspinfit.GetParError(myspinfit.GetParNumber("omega"))

    g, dg = calcLande(omega, domega, 3.5e-3)
    return g, dg


def calcLande(omega, domega, bfield, T_mu=0):
    """
    omega: freq in MHz
    bfield: Tesla
    T_mu: kinetic energy in MeV
    """
    # convert MHz → Hz
    omega = omega * 1e6
    domega = domega * 1e6

    # relativistic gamma
    m_mu = 105.658  # MeV/c^2
    gamma = 1 + T_mu / m_mu

    omega_proper = omega * gamma  # correttamente per il tempo proprio

    # constants
    hbar = 1.054e-34  # J s
    q = 1.602e-19     # C
    c = 2.998e8       # m/s
    m = 105.658e6 * q / c**2  # kg
    magneton = q * hbar / (2 * m)

    g = omega_proper * hbar / (magneton * bfield)
    dg = g * (domega / omega)  # errore relativo invariato

    print(f"Lande g (relativistic) = {g:.6f} ± {dg:.6f}")
    return g, dg

if __name__=='__main__':
    import numpy as np
    base_path = "/home/ubuntu/SMRR/Esercizi/Task3/task3b/Results_PbWO4/Muons/WITH_B"
    energies = np.arange(10, 110, 10)  # 10 energies from 10 MeV to 100 MeV

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

    gr.SetTitle("Electron g-factor vs Energy;Energy [MeV];g")
    gr.SetMarkerStyle(20)
    gr.SetMarkerSize(1.2)
    theoretical_g = 2.002319
    mean_g = sum(g_vals) / len(g_vals)
    # Imposta il range Y prima di Draw
    ymin = min(min(g_vals), theoretical_g, mean_g) * 0.99
    ymax = max(max(g_vals), theoretical_g, mean_g) * 1.01
    gr.GetYaxis().SetRangeUser(ymin, ymax)

    gr.Draw("AP")  # P = marker, A = axis
    '''
    # --- mean line in red ---
    line_mean = TLine(min(energies), mean_g, max(energies), mean_g)
    line_mean.SetLineColor(kRed)
    line_mean.SetLineStyle(2)
    line_mean.SetLineWidth(2)
    line_mean.Draw("same")  # important: "same"
    
    # --- theoretical line in blue ---
    line_theory = TLine(min(energies), theoretical_g, max(energies), theoretical_g)
    line_theory.SetLineColor(kBlue)
    line_theory.SetLineStyle(2)
    line_theory.SetLineWidth(2)
    line_theory.Draw("same")
    '''
    # --- legend ---
    leg = TLegend(0.15, 0.75, 0.4, 0.9)
    leg.AddEntry(gr, "Measured g", "p")
    #leg.AddEntry(line_mean, "Computed mean g", "l")
    #leg.AddEntry(line_theory, "Theoretical g", "l")
    leg.Draw("same")

    cG.SaveAs(os.path.join(save_path, "g_vs_energy_errors.png"))
    cG.SaveAs(os.path.join(save_path, "g_vs_energy_errors.pdf"))
    cG.Close()