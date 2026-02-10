import sys
import os
sys.path.append("/opt/root/lib")

from ROOT import TCanvas, TFile, gROOT

folder_path_muons_NOB = "/home/ubuntu/SMRR/Exercises/task3/task3b_build/Muons/No_B_field/"
folder_path_muons_B   = "/home/ubuntu/SMRR/Exercises/task3/task3b_build/Muons/With_B_field/"
folder_path_pions     = "/home/ubuntu/SMRR/Exercises/task3/task3b_build/Pions/"

folders = [
    folder_path_muons_NOB,
    folder_path_muons_B,
    folder_path_pions
]

gROOT.SetStyle("Plain")
gROOT.SetBatch(True)   # fondamentale: niente finestre interattive

for folder in folders:
    print(f"\nProcessing folder: {folder}")

    for fname in os.listdir(folder):
        if not fname.endswith(".root"):
            continue

        fullpath = os.path.join(folder, fname)
        print(f"  -> Processing file: {fname}")

        f = TFile.Open(fullpath)
        if not f or f.IsZombie():
            print("     [ERROR] Cannot open file")
            continue

        # Canvas
        c1 = TCanvas("c1", "Decay analysis", 1000, 800)
        c1.Divide(2, 2)

        c1.cd(1)
        if hasattr(f, "decayPos"):
            f.decayPos.Draw()

        c1.cd(2)
        if hasattr(f, "decayTime"):
            f.decayTime.Draw()

        c1.cd(3)
        if hasattr(f, "decayTimeForward"):
            f.decayTimeForward.Draw()

        c1.cd(4)
        if hasattr(f, "decayTimeBackward"):
            f.decayTimeBackward.Draw()

        c1.Update()

        # Nome output (stesso nome del ROOT file)
        base_name = os.path.splitext(fname)[0]
        out_png = os.path.join(folder, base_name + "_decayPlots.png")
        out_pdf = os.path.join(folder, base_name + "_decayPlots.pdf")

        c1.SaveAs(out_png)
        c1.SaveAs(out_pdf)

        print(f"     Saved: {out_png}")
        print(f"     Saved: {out_pdf}")

        f.Close()
        del c1