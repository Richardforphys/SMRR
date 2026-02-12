import ROOT
import os
from collections import defaultdict

ROOT.gROOT.SetBatch(True)

base_path = "/home/ubuntu/SMRR/Exercises/mytask6_build/RUNS/NEUTRONS/PE"
energy_dirs = ["0.5MeV", "1MeV", "2MeV", "5MeV", "7MeV", "10MeV"]

tree_name = "Gas_Tree;1"
branch_name = "mtype"

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