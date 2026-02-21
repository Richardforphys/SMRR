#!/usr/bin/python -i
#  $Id:$

##@file analyse.py
# Simple analyse script.
#
# PyROOT script for displaying decay time distributions.

import sys
sys.path.append("/opt/root/lib")

from ROOT import TCanvas, TFile, gROOT, gApplication

gROOT.SetStyle('Plain')

file = TFile.Open('run_0.root')

file.ls()

c1 = TCanvas();
c1.Divide(2,2)
c1.cd(1)
file.decayPos.Draw()
c1.cd(2)
file.decayTime.Draw()
c1.cd(3)
file.decayTimeForward.Draw()
c1.cd(4)
file.decayTimeBackward.Draw()
c1.Modified()
c1.Update()

#gApplication.Run()

print "\n to quit press Ctrl-D"
