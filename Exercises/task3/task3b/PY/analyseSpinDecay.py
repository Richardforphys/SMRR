import sys
sys.path.append("/opt/root/lib")

from math import *  #sqrt, exp, cos, pi

from ROOT import TCanvas, TF1, TFile, TGaxis
from ROOT import gROOT, gStyle, gApplication
from ROOT import kRed

# set some global style options
gROOT.SetStyle('Plain')
gStyle.SetOptFit(1)
TGaxis.SetMaxDigits(3)

## define simple decay fit function
def decay(x,par):
    
    return par[0]*exp(-x[0]/par[1])

## define fit function with spin dependence
def spinDecay(x,par):
  
    return par[0]*exp(-x[0]/par[1])*(1 + par[2]*cos(par[3]*x[0] + par[4]) )

# set fit range, parameter names and start values
myfit = TF1('myfit', decay, 0., 20.e3,2)
myfit.SetParName(0,'A')
myfit.SetParameter(0,10000.)
myfit.SetParName(1,'tau')
myfit.SetParameter(1,200.)

# set fit line style
myfit.SetLineColor(kRed)
myfit.SetLineWidth(1)

# set fit range, parameter names and start values
myspinfit = TF1('myspinfit', spinDecay, 0., 20.e3,5)
myspinfit.SetParName(0,'A')
myspinfit.SetParameter(0,10000.)
myspinfit.SetParName(1,'tau')
myspinfit.SetParameter(1,2000.)
myspinfit.SetParName(2,'B')
myspinfit.SetParameter(2,0.1)
myspinfit.SetParName(3,'omega')
myspinfit.SetParameter(3,3.)
myspinfit.SetParName(4,'delta')
myspinfit.SetParameter(4,0)

# set fit line style
myfit.SetLineColor(kRed)
myfit.SetLineWidth(1)
myspinfit.SetLineColor(kRed)
myspinfit.SetLineWidth(1)

"""Read root file.

Simple PyROOT macro to read a root file and plot
dacay time and decay position.
"""
def analyseDecay(fname):
    global file, c1, c2, c3
    
    # load histograms from file
    file = TFile.Open(fname)
    #file.ls()

    # draw histogram and fit
    c1 = TCanvas('c1','Decay Time',10,10,700,500)
    file.decayTime.Draw()
    file.decayTime.Fit(myfit)
    c1.Modified()
    c1.Update()

    c2 = TCanvas('c2','Decay Position',250,20,700,500)
    file.decayPos.Draw()

    # print out results
    print ('average Decay time =',file.decayTime.GetMean(),'us')
    print ('fitted Decay time = (',myfit.GetParameter('tau'),'+-',myfit.GetParError(1),') us')
    print ('average Decay position =',file.decayPos.GetMean(),'mm')

    # draw histogram and fit
    c3 = TCanvas('c3','Decay Time Forward',30,50,700,500)
    file.decayTimeForward.Draw()
    myspinfit.FixParameter(0,myfit.GetParameter('A')/2.)
    myspinfit.FixParameter(1,myfit.GetParameter('tau'))
    file.decayTimeForward.Fit(myspinfit)
    c3.Modified()
    c3.Update()
    
    calcLande(myspinfit.GetParameter('omega'),3.5e-3)  # tesla

def calcLande(omega,bfield):
    omega=omega*1.e6     # MHz
    hbar=1.054e-34  # J s
    q=1.602e-19     #  C
    c=2.998e8 # m/s
    m=105.658e6*q/c**2
    magneton=q*hbar/(2.*m)  # 4.485e-26 * J/T
   
    g=omega*hbar/(magneton*bfield)

    print ('Lande g=',g)
    return g


if __name__=='__main__':
    fname=r"/home/ubuntu/SMRR/Exercises/task3/task3b_build/WITH_B/PIONS/run_8.root"
    # check for run time arguments
    if len(sys.argv)>1:
        fname=sys.argv[1]
    analyseDecay(fname)

    c1.Modified()
    c1.Update()
    gApplication.Run()

print ("\n to quit press Ctrl-D")
