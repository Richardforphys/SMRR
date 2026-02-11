import sys
import os
import re
from ROOT import TCanvas, TFile, gROOT, TGraphErrors

# ---------------- Paths ----------------
folders = {
    "muons_noB":  "/home/ubuntu/SMRR/Exercises/task3/task3b_build/WITHOUT_B/MUONS",
    "muons_B":    "/home/ubuntu/SMRR/Exercises/task3/task3b_build/WITH_B/MUONS",
    "pions_noB":  "/home/ubuntu/SMRR/Exercises/task3/task3b_build/WITHOUT_B/PIONS",
    "pions_B":    "/home/ubuntu/SMRR/Exercises/task3/task3b_build/WITH_B/PIONS"
}

energies = [10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100]
save_path = "/home/ubuntu/SMRR/Exercises/task3/task3b_build/DECAY_POS_GRAPHS"
os.makedirs(save_path, exist_ok=True)

# ---------------- ROOT style ----------------
gROOT.SetStyle("Plain")
gROOT.SetBatch(True)

# ---------------- Loop on folders ----------------
for tag, folder in folders.items():
    print(f"\n=== Processing folder: {folder} ===")
    graph = TGraphErrors()
    graph.SetName(f"gDecayPos_{tag}")
    graph.SetTitle(f"Mean decay position vs Energy ({tag});Energy [MeV];Mean decay position [mm]")

    # Sort files numerically by extracting run number
    def run_number(fname):
        m = re.search(r'run_(\d+)\.root', fname)
        return int(m.group(1)) if m else -1

    files = sorted([f for f in os.listdir(folder) if f.endswith(".root")], key=run_number)
    if not files:
        print(f"  [WARNING] No ROOT files found in {folder}")
        continue
    
    for i, fname in enumerate(files):
        fullpath = os.path.join(folder, fname)
        f = TFile.Open(fullpath)
        if not f or f.IsZombie():
            print(f"  [ERROR] Cannot open {fname}")
            continue

        h = f.Get("decayPos")
        if not h or h.GetEntries() == 0:
            print(f"  [WARNING] {fname} has no valid histogram")
            f.Close()
            continue

        mean  = h.GetMean() + 230
        emean = h.GetMeanError()
        rms   = h.GetRMS()

        if i >= len(energies):
            print(f"  [WARNING] File index {i} exceeds energies list, skipping")
            f.Close()
            continue
        energy = energies[i]

        idx = graph.GetN()
        graph.SetPoint(idx, energy, mean)
        graph.SetPointError(idx, 0.0, emean)

        print(f"  {fname}: E={energy} MeV, mean={mean:.2f} ± {emean:.2f} mm, RMS={rms:.2f} mm")
        f.Close()

    # ---------------- Draw & Save ----------------
    if graph.GetN() == 0:
        print(f"  [WARNING] No points to plot for {tag}")
        continue

    c = TCanvas(f"c_{tag}", "", 800, 600)
    graph.SetMarkerStyle(20)
    graph.SetMarkerSize(1.2)
    graph.Draw("AP")

    png = os.path.join(save_path, f"meanDecayPos_vs_E_{tag}.png")
    pdf = os.path.join(save_path, f"meanDecayPos_vs_E_{tag}.pdf")
    rootfile = os.path.join(save_path, f"meanDecayPos_vs_E_{tag}.root")

    c.SaveAs(png)
    c.SaveAs(pdf)

    fout = TFile(rootfile, "RECREATE")
    graph.Write()
    fout.Close()

    print(f"Saved:")
    print(f"  {png}")
    print(f"  {pdf}")
    print(f"  {rootfile}")

    c.Close()
    del c