#!/usr/bin/env python -i
#!/usr/bin/python -i
#  $Id:$

##@file analyse.py
# Simple Analyse
#
# Simple PyROOT for analysing the root file.
# 

import sys
sys.path.append("/opt/root/lib")

from math import sqrt, exp

from ROOT import TCanvas, TF1, TFile, TGaxis
from ROOT import gROOT, gStyle, gApplication
from ROOT import kBlack, kWhite, kRed, kBlue, kGreen, kYellow, kMagenta, kCyan, kTRUE, kFALSE

# set some global style options
gROOT.SetStyle('Plain')
gStyle.SetOptFit(1)
TGaxis.SetMaxDigits(3)

idx=1
colors=[0,kBlack,kRed,kBlue,kGreen,kYellow+1,kMagenta, kCyan,kBlack]
files=[]


## perform analysis
def analyse(fname):
    global files, c1, c2, c3, idx

    if idx>8: 
        return
    dopt=""
    if idx>1:
        dopt="same"
    else:
        c1 = TCanvas('a','Total Energy',10,10,700,500)
        c2 = TCanvas('b','Central Crystal',250,30,700,500)
        c3 = TCanvas('c','Energy Profile',50,50,700,500)

    print " reading ",idx," file ",fname

    # load histograms from file
    f = TFile.Open(fname)

    # create histogram
    c1.cd()
    f.etot.SetLineColor(colors[idx])
    f.etot.Draw(dopt)
    c2.cd()
    f.e0.SetLineColor(colors[idx])
    f.e0.Draw(dopt)
    c3.cd()
    f.ez.SetLineColor(colors[idx])
    f.ez.Draw(dopt)
    idx+=1

    files.append(f)

    # print out results
    print "  Normalized energy in EM calo:         ",f.etot.GetMean(),
    print "  RMS: ",f.etot.GetRMS()
    print "  Normalized energy in central crystal: ",f.e0.GetMean(),
    print "  RMS: ",f.e0.GetRMS()
    print "  Ratio of central crystal to total:    ",f.e0.GetMean()/f.etot.GetMean()

if __name__=='__main__':
    fname='run_0.root'
    # check for run time arguments
    if len(sys.argv)>1:
        for i in  range(1,len(sys.argv)):
            fname=sys.argv[i]
            analyse(fname)
    else:
        analyse(fname)

    #gApplication.Run()

print "\n to quit press Ctrl-D"
