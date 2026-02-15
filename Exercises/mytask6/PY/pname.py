import ROOT
import os
from collections import defaultdict

ROOT.gROOT.SetBatch(True)

base_path = "/home/ricca/SMRR/Exercises/mytask6/RUNS/PIONS/PE/Energy"
energy_dirs = ["0.1MeV","0.5MeV", "1MeV", "3MeV", "5MeV", "7MeV", "10MeV"]

tree_name = "Gas_Tree;1"
branch_name = "type"

pdg_db = ROOT.TDatabasePDG.Instance()
pdg_per_energy = {}

for e_dir in energy_dirs:

    file_path = os.path.join(base_path, e_dir, "tree_run0.root")
    f = ROOT.TFile.Open(file_path)
    if not f or f.IsZombie():
        raise RuntimeError(f"Errore apertura {file_path}")

    tree = f.Get(tree_name)
    if not tree:
        raise RuntimeError(f"TTree {tree_name} non trovato in {file_path}")

    leaf = tree.GetLeaf(branch_name)
    if not leaf:
        raise RuntimeError(f"Leaf {branch_name} non trovata")

    pdg_counts = defaultdict(int)

    for i in range(tree.GetEntries()):
        tree.GetEntry(i)
        pdg = int(leaf.GetValue())
        pdg_counts[pdg] += 1

    pdg_per_energy[e_dir] = dict(pdg_counts)
    f.Close()

for e, counts in pdg_per_energy.items():
    print(f"\nEnergy {e}")
    for pdg, n in sorted(counts.items()):
        particle = pdg_db.GetParticle(pdg)
        name = particle.GetName() if particle else "unknown"
        print(f"  {name:12s} ({pdg:10d}): {n}")

for e_dir, counts in pdg_per_energy.items():

    h = ROOT.TH1I(
        f"h_pdg_{e_dir}",
        f"{e_dir};PDG code;Counts",
        20000, -5000, 20000
    )

    for pdg, n in counts.items():
        h.Fill(pdg, n)

    h.SetDirectory(0)

# =========================
# HISTOGRAMS: PARTICLE TYPES
# =========================

outdir = "pions_pdg_histograms"
os.makedirs(outdir, exist_ok=True)

for e_dir, counts in pdg_per_energy.items():

    pdgs = sorted(counts.keys())
    nbins = len(pdgs)

    h = ROOT.TH1I(
        f"h_pdg_{e_dir}",
        f"{e_dir};Particle;Counts",
        nbins, 0, nbins
    )

    # Riempimento + label asse X
    for i, pdg in enumerate(pdgs, start=1):
        h.SetBinContent(i, counts[pdg])
        particle = pdg_db.GetParticle(pdg)
        name = particle.GetName() if particle else str(pdg)
        h.GetXaxis().SetBinLabel(i, name)

    h.SetLineWidth(2)
    h.SetFillColor(ROOT.kAzure - 9)

    # Canvas
    c = ROOT.TCanvas(f"c_{e_dir}", "", 900, 600)
    c.SetBottomMargin(0.25)  # spazio per label
    h.Draw("HIST")

    c.SaveAs(f"{outdir}/pdg_{e_dir}.pdf")

    # =========================
# OPTION 3: HEATMAP (Energy vs Particle Type)
# =========================

# Colleziona tutti i PDG presenti
all_pdgs = set()
for counts in pdg_per_energy.values():
    all_pdgs.update(counts.keys())

# Ordiniamo i PDG per nome (più leggibile)
pdg_db = ROOT.TDatabasePDG.Instance()
pdg_list = sorted(
    all_pdgs,
    key=lambda pdg: pdg_db.GetParticle(pdg).GetName()
    if pdg_db.GetParticle(pdg) else str(pdg)
)

energy_list = energy_dirs

# Istogramma 2D categoriale
h2 = ROOT.TH2F(
    "h2_particle_energy",
    ";Neutron energy;Particle type",
    len(energy_list), 0, len(energy_list),
    len(pdg_list), 0, len(pdg_list)
)

# Label asse X (energie)
for i, e in enumerate(energy_list, start=1):
    h2.GetXaxis().SetBinLabel(i, e)

# Label asse Y (particelle)
for j, pdg in enumerate(pdg_list, start=1):
    particle = pdg_db.GetParticle(pdg)
    name = particle.GetName() if particle else str(pdg)
    h2.GetYaxis().SetBinLabel(j, name)

# Riempimento: FRAZIONE per energia
for ix, e in enumerate(energy_list, start=1):
    counts = pdg_per_energy[e]
    total = sum(counts.values())
    if total == 0:
        continue

    for iy, pdg in enumerate(pdg_list, start=1):
        frac = counts.get(pdg, 0) / total
        h2.SetBinContent(ix, iy, frac)

# =========================
# DRAW
# =========================
c = ROOT.TCanvas("c_heatmap", "", 1000, 700)
c.SetRightMargin(0.15)
c.SetBottomMargin(0.15)

ROOT.gStyle.SetPaintTextFormat(".2f")

h2.SetStats(0)
h2.Draw("COLZ TEXT")

c.SaveAs("pions_particle_energy_heatmap.pdf")