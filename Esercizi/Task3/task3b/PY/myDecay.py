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

def analyseDecay(fname):
    f = TFile.Open(fname)

    h = f.Get("decayTime")

    h.Fit(fit)  # Q = quiet, N = no draw

    tau     = fit.GetParameter(1)
    tau_err = fit.GetParError(1)
    
    c = TCanvas("c", "", 700, 500)

    h.SetTitle("Decay time distribution;Time [ms];Counts")
    h.Draw("E")           # histogram with errors
    fit.Draw("same")      # exponential fit

    outname = f"decay_fit_{tag}.png"
    c.SaveAs(outname)

    c.Close()
    f.Close()
    return tau, tau_err

# ------------------ Paths ------------------

datasets = {
    "WITH_B":    "/home/ubuntu/SMRR/Exercises/task3/task3b_build/WITH_B/MUONS/",
    "WITHOUT_B": "/home/ubuntu/SMRR/Exercises/task3/task3b_build/WITHOUT_B/MUONS/"
}

# ------------------ Loop over all files ------------------

tau_all     = []
tau_err_all = []
labels      = []

for tag, path in datasets.items():
    for fname in sorted(os.listdir(path)):
        fullpath = os.path.join(path, fname)
        tau, tau_err = analyseDecay(fullpath)

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
    label=rf'Weighted mean = {tau_mean:.2f} ms'
)

plt.axvspan(
    tau_mean - tau_sigma,
    tau_mean + tau_sigma,
    color='red',
    alpha=0.25,
    label=rf'$1\sigma = {tau_sigma:.2f}$ ms'
)

plt.xlabel(r'$\tau$ [ms]', fontsize=13)
plt.ylabel('Counts', fontsize=13)
plt.title('Muon lifetime – all datasets', fontsize=14)

plt.grid(True, ls='--', alpha=0.4)
plt.legend()
plt.tight_layout()

plt.savefig("tau_histogram_all.png", dpi=300)
plt.savefig("tau_histogram_all.pdf")
plt.show()

# x-axis: run index
x = np.arange(len(tau_all))

# Split datasets
mask_B   = labels == "WITH_B"
mask_noB = labels == "WITHOUT_B"

plt.figure(figsize=(8, 5))

# WITH B
plt.errorbar(
    x[mask_B],
    tau_all[mask_B],
    yerr=tau_err_all[mask_B],
    fmt='o',
    capsize=3,
    label='WITH B'
)

# WITHOUT B
plt.errorbar(
    x[mask_noB],
    tau_all[mask_noB],
    yerr=tau_err_all[mask_noB],
    fmt='s',
    capsize=3,
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
    label=rf'Weighted mean = {tau_mean:.2f} ms'
)

plt.axhspan(
    tau_mean - tau_sigma,
    tau_mean + tau_sigma,
    color='red',
    alpha=0.25,
    label=rf'$1\sigma = {tau_sigma:.2f}$ ms'
)

plt.axhline(
    tau_all[mask_B].mean(),
    color='blue',
    lw=2,
    ls='--',
    label=rf'WITH B mean = {tau_all[mask_B].mean():.2f} ms'
)

plt.axhline(
    tau_all[mask_noB].mean(),
    color='green',
    lw=2,
    ls='--',
    label=rf'WITHOUT B mean = {tau_all[mask_noB].mean():.2f} ms'
)
# Labels & style
plt.xlabel('Run index', fontsize=13)
plt.ylabel(r'$\tau$ [ms]', fontsize=13)
plt.title('Muon lifetime per run', fontsize=14)

plt.grid(True, ls='--', alpha=0.4)
plt.legend()
plt.tight_layout()

plt.savefig("tau_scatter.png", dpi=300)
plt.savefig("tau_scatter.pdf")
plt.show()