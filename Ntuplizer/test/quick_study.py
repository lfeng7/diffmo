from fwlite_boilerplate import *

from optparse import OptionParser

import sys

# Job steering

# Input inputFiles to use. This is in "glob" format, so you can use wildcards.
# If you get a "cannot find file" type of error, be sure to use "\*" instead
# of "*" to make sure you don't confuse the shell. 

parser = OptionParser()

parser.add_option('--inputfiles', metavar='F', type='string', action='store',
                  default = "none",
                  dest='inputFiles',
                  help='Input files')


(options, args) = parser.parse_args()

argv = []

#debug
print options.inputFiles

# Get the inputfiles.
if options.inputFiles != 'none':
    files = glob.glob( options.inputFiles )

files = ['TT_CT10_TuneZ2star_8TeV-powheg-tauola_TLBSM_PAT.root']

# Read input files
events = Events(files)

# Control constants
nevt_cut = 1000

# Handles and labels
PatElePF_hndl = Handle('vector<pat::Electron>')
PatElePF_label = ('selectedPatElectronsPFlow')


# Book histograms
h1 = ROOT.TH1D('jetspt','jetspt;pt GeV;events',50,0.,300.0)


# Counter initiation 
n_evt = 0
n_evt_csv = 0

print 'Getting',events.size(),'events'
# Event loop
for evt in events:

    # counting and stuff
    if n_evt == nevt_cut: break
    #print 'loop over',n_evt,'events'
    n_evt += 1
    if n_evt%5000 == 1: print 'Loop over',n_evt,'event'

    evt.getByLabel(PatElePF_label,PatElePF_hndl)
    els = PatElePF_hndl.product()
    if els.size() == 0 : continue
    else :
        el = els[0];break
    

# End of event loop

# Run summary
print 'break at event',n_evt


# Plotting and saving 
# histlist = [h_cutflow]

# plotting(histlist,event_type,"dump")
  
