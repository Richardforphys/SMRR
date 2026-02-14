import ROOT
import os
import math

ROOT.gROOT.SetBatch(True)  # niente GUI

# binning istogrammi
nbins = 100
xmin = 10
xmax = 1000

# =========================
# CONFIGURAZIONE
# =========================

base_path = "/home/ubuntu/SMRR/Exercises/mytask6_build/RUNS/NEUTRONS/PE"

energy_dirs = [
    "0.5MeV",
    "1MeV",
    "2MeV",
    "5MeV",
    "7MeV",
    "10MeV"
]

tree_name = "Gas_Tree;1"        # <-- cambia se il tuo TTree ha nome diverso
branch_name = "medep"      # <-- cambia con il branch che vuoi plottare

# =========================
# CICLO SUI FILE
# =========================

energies_numeric = []
entries_list = []
histograms = []

for e_dir in energy_dirs:

    file_path = os.path.join(base_path, e_dir, "tree_run0.root")
    f = ROOT.TFile.Open(file_path)

    if not f or f.IsZombie():
        print(f"Errore apertura {file_path}")
        continue

    tree = f.Get(tree_name)
    if not tree:
        print(f"TTree {tree_name} non trovato in {file_path}")
        continue

    n_entries = tree.GetEntries()
    entries_list.append(n_entries)

    # parsing energia numerica
    if "KeV" in e_dir:
        value = float(e_dir.replace("KeV","")) * 1e-3
    elif "MeV" in e_dir:
        value = float(e_dir.replace("MeV",""))
    else:
        value = 0

    energies_numeric.append(value)

    h = ROOT.TH1D(
    f"mother_h_{e_dir}",
    f"{e_dir};{branch_name};Counts",
    nbins, xmin, xmax
    )

    tree.Draw(f"{branch_name}>>mother_h_{e_dir}", "", "goff")

    h.SetDirectory(0)   
    h.SetLineWidth(2)
    h.SetLineColor(ROOT.kBlack)

    histograms.append(h)
    f.Close()

# =========================
# 2️⃣ Istogrammi singoli "da paper"
# =========================

for h in histograms:
    print(h.GetName())
    c = ROOT.TCanvas(f"c_{h.GetName()}", "", 800, 600)
    h.Draw("hist")
    c.SaveAs(f"{h.GetName()}.pdf")