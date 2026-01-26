#!/usr/bin/python -i
#  $Id:$

##@file analyseDecay.py
# Analyse decays.
#
# Simple PyROOT for analysing the root file.
# Make a simple fit.

import sys
sys.path.append("/opt/root/lib")

from math import sqrt, exp

from ROOT import TCanvas, TF1, TFile, TGaxis
from ROOT import gROOT, gStyle, gApplication
from ROOT import kRed, kTRUE

# set some global style options
gROOT.SetStyle('Plain')
gStyle.SetOptFit(1)
TGaxis.SetMaxDigits(3)


## define fit function
def decay(x,par):
    return par[0]*exp(-x[0]/par[1])

# set fit range, parameter names and start values
myfit = TF1('myfit', decay, 0., 20.e3,2)
myfit.SetParName(0,'A')
myfit.SetParameter(0,10.)
myfit.SetParName(1,'tau')
myfit.SetParameter(1,200.)

# set fit line style
myfit.SetLineColor(kRed)
myfit.SetLineWidth(1)

## perform analysis
def analyseDecay(fname):
    global file, c1, c2
    
    # load histograms from file
    file = TFile.Open(fname)
    #file.ls()

    # draw histogram and fit
    c1 = TCanvas('c1','Decay Time',10,10,700,500)
    file.decayTime.Draw()
    file.decayTime.Fit(myfit);
    c1.Modified()
    c1.Update()

    c2 = TCanvas('c2','Decay Position',250,20,700,500)
    file.decayPos.Draw()

    # print out results
    print 'average Decay time =',file.decayTime.GetMean(),'ms'
    print 'fitted Decay time = (',myfit.GetParameter('tau'),'+-',myfit.GetParError(1),') ms'
    print 'average Decay position =',file.decayPos.GetMean(),'mm'


if __name__=='__main__':
    fname='run_0.root'
    # check for run time arguments
    if len(sys.argv)>1:
        fname=sys.argv[1]
    analyseDecay(fname)

    #c1.Modified()
    #c1.Update()
    #gApplication.Run()

print "\n to quit press Ctrl-D"
