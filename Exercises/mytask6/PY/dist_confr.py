import ROOT
import os

ROOT.gROOT.SetBatch(True)  # niente GUI

# =========================
# CONFIGURAZIONE
# =========================
data_paths = {
    "PE": "/home/ricca/SMRR/Exercises/mytask6/RUNS/PIONS/PE/Distance",
    #"Carbon": "/home/ricca/SMRR/Exercises/mytask6/MATERIALS/CARBON/Distance",
    "Gold": "/home/ricca/SMRR/Exercises/mytask6/RUNS/PIONS/GOLD/Distance",
    "Copper": "/home/ricca/SMRR/Exercises/mytask6/RUNS/PIONS/COPPER/Distance"
}

dist_dirs = ["0.1cm", "0.5cm", "1cm","5cm", "7cm", "10cm"]
branch_name = "edep"

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
# RACCOLTA DATI
# =========================
results = {}
for material, path in data_paths.items():
    energies = []
    entries = []
    if "PE" in material:
        tree_name = "Gas_Tree;1"
    else:
        tree_name = "Gas_Tree;1"
    for d_dir in dist_dirs:
        file_path = os.path.join(path, d_dir, "tree_run0.root")
        f = ROOT.TFile.Open(file_path)
        if not f or f.IsZombie():
            print(f"Errore apertura {file_path}")
            continue
        tree = f.Get(tree_name)
        if not tree:
            print(f"TTree {tree_name} non trovato in {file_path}")
            f.Close()
            continue
        energies.append(float(d_dir.replace("cm", "")))
        entries.append(tree.GetEntries())
        f.Close()
    
    # Ordina per distanza
    sorted_data = sorted(zip(energies, entries))
    energies_sorted, entries_sorted = zip(*sorted_data)
    results[material] = {"energies": energies_sorted, "entries": entries_sorted}

# =========================
# CREAZIONE TMultiGraph
# =========================
mg = ROOT.TMultiGraph()

# Colori e marker
styles = {
    "PE":   {"color": ROOT.kRed,    "marker": 22},
    #"Carbon":{"color": ROOT.kBlack,  "marker": 23},
    "Gold": {"color": ROOT.kOrange,"marker": 29},
    "Copper":{"color": ROOT.kBlue,   "marker": 33}
}

graphs = {}

for material in results:
    energies = results[material]["energies"]
    entries = results[material]["entries"]
    g = ROOT.TGraph(len(energies))
    for i in range(len(energies)):
        g.SetPoint(i, energies[i], entries[i])
    g.SetLineColor(styles[material]["color"])
    g.SetMarkerColor(styles[material]["color"])
    g.SetMarkerStyle(styles[material]["marker"])
    g.SetMarkerSize(2)
    g.SetLineWidth(3)
    mg.Add(g, "APL")
    graphs[material] = g

# =========================
# CANVAS E DRAW
# =========================
c1 = ROOT.TCanvas("c1", "", 800, 600)
c1.SetLogx()
c1.SetLogy()
mg.SetTitle(";Distance [cm];Number of entries")
mg.Draw("APL")  # TMultiGraph gestisce tutto

# Legenda
leg = ROOT.TLegend(0.25, 0.4, 0.45, 0.58)
for material, g in graphs.items():
    leg.AddEntry(g, material, "lp")
leg.Draw()

c1.SaveAs("pions_entries_vs_distance.pdf")