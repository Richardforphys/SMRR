import ROOT
import os
import math

ROOT.gROOT.SetBatch(True)  # niente GUI

# =========================
# CONFIGURAZIONE
# =========================

base_path = "/home/ricca/SMRR/Exercises/mytask6/MATERIALS/PE/Distance"

energy_dirs = [
    "0.1MeV",
    "0.5MeV",
    "1MeV",
    "3MeV",
    "5MeV",
    "7MeV",
    "10MeV"
]

dist_dirs = [
    "0.1cm",
    "0.5cm",
    "1cm",
    "3cm",
    "5cm",
    "7cm",
    "10cm"
]

tree_name = "Gas_Tree;1"        # <-- cambia se il tuo TTree ha nome diverso
branch_name = "edep"      # <-- cambia con il branch che vuoi plottare

# binning istogrammi
nbins = 100
xmin = 0
xmax = 0.01

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

for d_dir in dist_dirs:

    file_path = os.path.join(base_path, d_dir, "tree_run0.root")
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

    # parsing distanza numerica
    value = float(d_dir.replace("cm",""))
    energies_numeric.append(value)

    h = ROOT.TH1D(
        f"h_{d_dir}",
        f"{d_dir};Energy deposited [MeV];Counts",
        nbins, xmin, xmax
    )

    tree.Draw(f"{branch_name}>>h_{d_dir}", "", "goff")

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

graph.SetTitle(";Distance [cm];Number of entries")
graph.SetMarkerStyle(20)
graph.SetMarkerSize(1.2)
graph.SetLineWidth(2)

c1 = ROOT.TCanvas("c1", "", 800, 600)
c1.SetLogx()
graph.Draw("APL")
c1.SaveAs("entries_vs_distance.pdf")


# =========================
# 2️⃣ Istogrammi singoli "da paper"
# =========================
'''
for h in histograms:
    print(h.GetName())
    c = ROOT.TCanvas(f"c_{h.GetName()}", "", 800, 600)
    h.Draw("hist")
    c.SaveAs(f"{h.GetName()}.pdf")
'''

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