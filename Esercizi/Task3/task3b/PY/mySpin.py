import sys
sys.path.append("/opt/root/lib")
import os
save_path = "/home/ubuntu/SMRR/Esercizi/Task3/task3b/Results_PbWO4/Muons_Spin_PNG"

if not os.path.exists(save_path):
    os.makedirs(save_path)
from math import *  #sqrt, exp, cos, pi
from ROOT import TCanvas, TF1, TFile, TGaxis, TGraph
from ROOT import gROOT, gStyle, gApplication
from ROOT import kRed
from ROOT import TGraph
from ROOT import TGraphErrors, TCanvas, TLine, TLegend, TBox
from ROOT import kBlue, kRed, kBlack, kGray

# set some global style options
gROOT.SetStyle('Plain')
gStyle.SetOptStat(0)
#gStyle.SetOptFit(0)
gStyle.SetOptFit(1)
TGaxis.SetMaxDigits(3)

## define simple decay fit function
def decay(x,par):
    
    return par[0]*exp(-x[0]/par[1])

## define fit function with spin dependence
def spinDecay(x,par):
    """
    par[0] = A
    par[1] = tau
    par[2] = B_asymmetry
    par[3] = g
    par[4] = delta
    par[5] = Bfield (Tesla)
    """

    # constants
    hbar = 1.054e-34
    q = 1.602e-19
    c = 2.998e8
    m = 105.658e6 * q / c**2
    magneton = q * hbar / (2 * m)

    omega = par[3] * magneton * par[5] / hbar

    return par[0]*exp(-x[0]/par[1])*(1 + par[2]*cos(omega*x[0] + par[4]))

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
myspinfit = TF1('myspinfit', spinDecay, 0., 20.e3,6)

myspinfit.SetParName(0,'A')
myspinfit.SetParameter(0,10000.)
myspinfit.SetParName(1,'tau')
myspinfit.SetParameter(1,2000.)
myspinfit.SetParName(2,'Asym')
myspinfit.SetParameter(2,0.1)
myspinfit.SetParName(3,'g')
myspinfit.SetParameter(3,2.0)
myspinfit.SetParName(4,'delta')
myspinfit.SetParameter(4,0)
myspinfit.SetParName(5,'B')
myspinfit.SetParameter(5,3.5)

myspinfit.FixParameter(5, 3.5)

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

    g  = myspinfit.GetParameter('g')
    dg = myspinfit.GetParError(myspinfit.GetParNumber("g"))

    print(f"Fitted g = {g:.6f} ± {dg:.6f}")

    return g, dg
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

    # --- create graph ---
    gr = TGraphErrors(len(energies))
    for i in range(len(energies)):
        gr.SetPoint(i, energies[i], g_vals[i])
        gr.SetPointError(i, 0.0, dg_vals[i])

    cG = TCanvas("c_g_vs_E", "", 800, 600)
    cG.SetMargin(0.12, 0.05, 0.12, 0.05)

    # ----- Graph style -----
    gr.SetTitle("")
    gr.SetMarkerStyle(20)
    gr.SetMarkerSize(1.2)
    gr.SetMarkerColor(kBlack)
    gr.SetLineColor(kBlack)

    gr.GetXaxis().SetTitle("Muon kinetic energy [MeV]")
    gr.GetYaxis().SetTitle("g_{#mu}")

    gr.GetXaxis().SetTitleSize(0.045)
    gr.GetYaxis().SetTitleSize(0.045)
    gr.GetXaxis().SetLabelSize(0.04)
    gr.GetYaxis().SetLabelSize(0.04)
    gr.GetYaxis().SetTitleOffset(1.2)

    # ----- Theoretical value -----
    theoretical_g = 2.0023318418

    # Media pesata
    import numpy as np
    weights = 1/np.array(dg_vals)**2
    mean_g = np.sum(np.array(g_vals)*weights)/np.sum(weights)
    mean_err = np.sqrt(1/np.sum(weights))

    # ----- Y range stretto -----
    ymin = min(min(g_vals), theoretical_g) - 3*max(dg_vals)
    ymax = max(max(g_vals), theoretical_g) + 3*max(dg_vals)
    gr.GetYaxis().SetRangeUser(ymin, ymax)

    gr.Draw("AP")

    # ----- Banda ±1σ media -----
    band = TBox(min(energies), mean_g-mean_err,
                max(energies), mean_g+mean_err)
    band.SetFillColorAlpha(kGray, 0.3)
    band.SetLineColor(0)
    band.Draw("same")

    # ----- Linea media -----
    line_mean = TLine(min(energies), mean_g,
                    max(energies), mean_g)
    line_mean.SetLineColor(kRed)
    line_mean.SetLineWidth(2)
    line_mean.Draw("same")

    # ----- Linea teorica -----
    line_theory = TLine(min(energies), theoretical_g,
                        max(energies), theoretical_g)
    line_theory.SetLineColor(kBlue)
    line_theory.SetLineStyle(2)
    line_theory.SetLineWidth(2)
    line_theory.Draw("same")

    # ----- Legend -----
    leg = TLegend(0.15, 0.75, 0.45, 0.9)
    leg.SetBorderSize(0)
    leg.SetFillStyle(0)
    leg.SetTextSize(0.035)

    leg.AddEntry(gr, "Data", "p")
    leg.AddEntry(line_mean, "Weighted mean", "l")
    leg.AddEntry(line_theory, "SM prediction", "l")
    leg.Draw()

    cG.SaveAs(os.path.join(save_path, "g_vs_energy.pdf"))
    cG.SaveAs(os.path.join(save_path, "g_vs_energy.png"))
    cG.Close()