import sys
sys.path.append("/opt/root/lib")

import os
import numpy as np
import matplotlib.pyplot as plt
from math import exp, sqrt

from ROOT import (
    TFile, TF1, TCanvas,
    gROOT, gStyle
)
# ------------------ ROOT style ------------------
gROOT.SetBatch(True)
gROOT.SetStyle("Plain")
gStyle.SetOptFit(0)

# ------------------ Fit function ------------------

def decay(x, par):
    return par[0] * exp(-x[0] / par[1])

fit = TF1("fit", decay, 0.0, 20e3, 2)
fit.SetParNames("A", "tau")
fit.SetParameters(10.0, 200.0)

# ------------------ Analysis function ------------------

#!/usr/bin/python -i
#  $Id:$

##@file analyseDecay.py
# Analyse decays.
#
# Simple PyROOT for analysing the root file.
# Make a simple fit.

import sys
sys.path.append("/opt/root/lib")
import os
from math import sqrt, exp
from ROOT import TCanvas, TF1, TFile, TGaxis, TLatex
from ROOT import gROOT, gStyle, gApplication
from ROOT import kRed, kTRUE

# set some global style options
gROOT.SetStyle('Plain')
gStyle.SetOptFit(1)
TGaxis.SetMaxDigits(3)


## define fit function
def decay(x,par):
    return par[0]*exp(-x[0]/par[1])

# set fit range, parameter names and start values
myfit = TF1('myfit', decay, 0., 20.e3,2)
myfit.SetParName(0,'A')
myfit.SetParameter(0,10.)
myfit.SetParName(1,'tau')
myfit.SetParameter(1,200.)

# set fit line style
myfit.SetLineColor(kRed)
myfit.SetLineWidth(1)

## perform analysis
def analyseDecay(fname, tag):
    f = TFile.Open(fname)
    if not f or f.IsZombie():
        print(f"File not found or invalid: {fname}")
        return None, None

    h = f.Get("decayTime")
    if not h:
        print(f"Histogram 'decayTime' not found in {fname}")
        f.Close()
        return None, None

    # Fit
    h.Fit(myfit, "Q")  # usa myfit
    tau     = myfit.GetParameter(1)
    tau_err = myfit.GetParError(1)
    A       = myfit.GetParameter(0)

    # Canvas
    c = TCanvas("c", "", 700, 500)
    h.SetTitle("Decay time distribution;Time [#mus];Counts")
    h.Draw("E")
    myfit.Draw("same")

    # Box con parametri
    text = TLatex()
    text.SetNDC()
    text.SetTextSize(0.035)
    #text.DrawLatex(0.65, 0.80, f"A = {A:.2f}")
    #text.DrawLatex(0.65, 0.75, f"tau = {tau:.2f} ± {tau_err:.2f} #mus")

    # Salvataggio
    save_path = r"/home/ubuntu/SMRR/Esercizi/Task3/task3b/Results_PbWO4/Electrons_PNG/"
    os.makedirs(save_path, exist_ok=True)
    outname = f"decay_fit_{tag}_{os.path.basename(fname).replace('.root','')}.png"
    c.SaveAs(save_path + outname)

    c.Close()
    f.Close()
    return tau, tau_err

# ------------------ Paths ------------------

datasets = {
    "WITH_B":    "/home/ubuntu/SMRR/Esercizi/Task3/task3b/Results_PbWO4/Electrons/WITH_B",
    "WITHOUT_B": "/home/ubuntu/SMRR/Esercizi/Task3/task3b/Results_PbWO4/Electrons/WITHOUT_B/"
}

# ------------------ Loop over all files ------------------

tau_all     = []
tau_err_all = []
labels      = []

for tag, path in datasets.items():
    for fname in sorted(os.listdir(path)):
        fullpath = os.path.join(path, fname)
        tau, tau_err = analyseDecay(fullpath, tag)

        tau_all.append(tau)
        tau_err_all.append(tau_err)
        labels.append(tag)

tau_all     = np.array(tau_all)
tau_err_all = np.array(tau_err_all)
labels      = np.array(labels)

# ------------------ Statistics ------------------

weights = 1.0 / tau_err_all**2
tau_mean = np.average(tau_all, weights=weights)
tau_sigma = sqrt(1.0 / np.sum(weights))

print("===================================")
print(f"Weighted tau = {tau_mean:.2f} ± {tau_sigma:.2f} ms")
print("===================================")

# ------------------ Matplotlib histogram ------------------

plt.figure(figsize=(7, 5))

plt.hist(
    tau_all,
    bins='auto',
    histtype='stepfilled',
    edgecolor='black',
    alpha=0.75
)

plt.axvline(
    tau_mean,
    color='red',
    lw=2,
    label=rf'Weighted mean = {tau_mean:.2f} $\mu$s'
)

plt.axvspan(
    tau_mean - tau_sigma,
    tau_mean + tau_sigma,
    color='red',
    alpha=0.25,
    label=rf'$1\sigma = {tau_sigma:.2f}$ $\mu$s'
)

plt.xlabel(r'$\tau$ [$\mu$s]', fontsize=13)
plt.ylabel('Counts', fontsize=13)
plt.title('Electrons lifetime – all datasets', fontsize=14)

plt.grid(True, ls='--', alpha=0.4)
plt.legend()
plt.tight_layout()

plt.savefig("tau_histogram_all_pions.png", dpi=300)
plt.savefig("tau_histogram_all_pions.pdf")
plt.show()

# x-axis: run index
x = np.arange(10,110,10)

# Split datasets
mask_B   = labels == "WITH_B"
mask_noB = labels == "WITHOUT_B"

plt.figure(figsize=(8, 5))

# WITH B
plt.errorbar(
    x,
    tau_all[mask_B],
    yerr=tau_err_all[mask_B],
    fmt='x',
    capsize=3,
    color='red',
    label='WITH B'
)

# WITHOUT B
plt.errorbar(
    x,
    tau_all[mask_noB],
    yerr=tau_err_all[mask_noB],
    fmt='.',
    capsize=3,
    color='navy',
    label='WITHOUT B'
)

# Weighted mean over ALL data
weights = 1.0 / tau_err_all**2
tau_mean = np.average(tau_all, weights=weights)
tau_sigma = np.sqrt(1.0 / np.sum(weights))

plt.axhline(
    tau_mean,
    color='red',
    lw=2,
    label=rf'Weighted mean = {tau_mean:.2f} $\mu$s'
)

plt.axhspan(
    tau_mean - tau_sigma,
    tau_mean + tau_sigma,
    color='red',
    alpha=0.25,
    label=rf'$1\sigma = {tau_sigma:.2f}$ $\mu$s'
)

plt.axhline(
    tau_all[mask_B].mean(),
    color='blue',
    lw=2,
    ls='--',
    label=rf'WITH B mean = {tau_all[mask_B].mean():.2f} $\mu$s'
)

plt.axhline(
    tau_all[mask_noB].mean(),
    color='green',
    lw=2,
    ls='--',
    label=rf'WITHOUT B mean = {tau_all[mask_noB].mean():.2f} $\mu$s'
)
# Labels & style
plt.xlabel('Energy [MeV]', fontsize=13)
plt.ylabel(r'$\tau$ [$\mu$s]', fontsize=13)
plt.title('Electrons lifetime per run', fontsize=14)

plt.grid(True, ls='--', alpha=0.4)
plt.legend()
plt.tight_layout()

plt.savefig("tau_scatter_Electrons.png", dpi=300)
plt.savefig("tau_scatter_Electrons.pdf")
plt.show()