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
nevt_cut = -1 #1000
n_passed_cut = -1

# Handles and labels
PatElePF_hndl = Handle('vector<pat::Electron>')
PatElePF_label = ('selectedPatElectronsPFlow')
PV_hndl = Handle('std::vector<reco::Vertex>')
PV_label = ('goodOfflinePrimaryVertices')
#output file
fout = ROOT.TFile('slimmed_pat.root','recreate')
#fout.SetCompressionLevel(9)
outputtree = ROOT.TTree('pat','pat')

# Data

# set up vector containers
mva_vec = ROOT.vector('float')()
isEBEEGap_vec = ROOT.vector('int')()
pdgid_vec = ROOT.vector('int')()
passConversionVeto = ROOT.vector('int')()

all_vecs = [mva_vec,isEBEEGap_vec,pdgid_vec,passConversionVeto]
all_branch_names = ['mva','isEBEEGap','pdgid','passConversionVeto']
branches = zip(all_branch_names,all_vecs)
for ibr in branches:
    outputtree.Branch(ibr[0],ibr[1])

# Counter initiation 
n_evt = 0
n_evt_csv = 0
n_passed = 0

print 'Getting',events.size(),'events'
# Event loop
for evt in events:

    for ivec in all_vecs: ivec.clear()
    # counting and stuff
    if n_evt == nevt_cut or n_passed == n_passed_cut: break
    #print 'loop over',n_evt,'events'
    n_evt += 1
    if n_evt%5000 == 1: print 'Loop over',n_evt,'event'

    evt.getByLabel(PatElePF_label,PatElePF_hndl)
    evt.getByLabel(PV_label,PV_hndl)
    els = PatElePF_hndl.product()
    pvs = PV_hndl.product() 
    if els.size() == 0 : continue
    n_passed += 1
    electron = els[0]
    break
    for el in els:
        mva_vec.push_back(el.electronID("mvaTrigV0"))
        if el.isEBEEGap():
            isEBEEGap_vec.push_back(1)
        else : 
            isEBEEGap_vec.push_back(0)
        if el.passConversionVeto:
            passConversionVeto.push_back(1)
        else :
            passConversionVeto.push_back(0)
        pdgid_vec.push_back(el.pdgId())
    outputtree.Fill()

fout.Write()
fout.Close()

# End of event loop

# Run summary
print 'break at event',n_evt


# Plotting and saving 
# histlist = [h_cutflow]

# plotting(histlist,event_type,"dump")
  
