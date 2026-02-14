import ROOT
import os
import math

ROOT.gROOT.SetBatch(True)  # niente GUI

# =========================
# CONFIGURAZIONE
# =========================

base_path = "/home/ricca/SMRR/Exercises/mytask6/MATERIALS/PE/Energy"

energy_dirs = [
    "0.1MeV",
    "0.5MeV",
    "1MeV",
    "3MeV",
    "5MeV",
    "7MeV",
    "10MeV"
]

tree_name = "Gas_Tree;1"        # <-- cambia se il tuo TTree ha nome diverso
branch_name = "edep"      # <-- cambia con il branch che vuoi plottare

# binning istogrammi
nbins = 100
xmin = 10
xmax = 1000

# =========================
# STILE "DA PAPER"
# =========================

def set_paper_style():
    ROOT.gStyle.SetOptStat(0)
    ROOT.gStyle.SetTitleFont(42, "XYZ")
    ROOT.gStyle.SetLabelFont(42, "XYZ")
    ROOT.gStyle.SetTitleSize(0.05, "XYZ")
    ROOT.gStyle.SetLabelSize(0.045, "XYZ")
    ROOT.gStyle.SetPadTickX(1)
    ROOT.gStyle.SetPadTickY(1)
    ROOT.gStyle.SetLineWidth(2)
    ROOT.gStyle.SetFrameLineWidth(2)

set_paper_style()

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
    f"h_{e_dir}",
    f"{e_dir};{branch_name};Counts",
    nbins, xmin, xmax
    )

    tree.Draw(f"{branch_name}>>h_{e_dir}", "", "goff")

    h.SetDirectory(0)   
    h.SetLineWidth(2)
    h.SetLineColor(ROOT.kBlack)

    histograms.append(h)
    f.Close()


# =========================
# 1️⃣ Grafico Entries vs Energia
# =========================

graph = ROOT.TGraph(len(energies_numeric))
for i in range(len(energies_numeric)):
    graph.SetPoint(i, energies_numeric[i], entries_list[i])

graph.SetTitle(";Energy [MeV];Number of entries")
graph.SetMarkerStyle(20)
graph.SetMarkerSize(1.2)
graph.SetLineWidth(2)

c1 = ROOT.TCanvas("c1", "", 800, 600)
c1.SetLogx()
graph.Draw("APL")
c1.SaveAs("entries_vs_energy.pdf")


# =========================
# 2️⃣ Istogrammi singoli "da paper"
# =========================

for h in histograms:
    print(h.GetName())
    c = ROOT.TCanvas(f"c_{h.GetName()}", "", 800, 600)
    h.Draw("hist")
    c.SaveAs(f"{h.GetName()}.pdf")


# =========================
# 3️⃣ Matrice di istogrammi
# =========================

n_hist = len(histograms)
ncols = 3
nrows = math.ceil(n_hist / ncols)

c_matrix = ROOT.TCanvas("c_matrix", "", 1200, 800)
c_matrix.Divide(ncols, nrows)

for i, h in enumerate(histograms):
    c_matrix.cd(i+1)
    h.Draw("hist")

c_matrix.SaveAs("matrix_histograms.pdf")

print("Analisi completata.")