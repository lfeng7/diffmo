from WMCore.Configuration import Configuration
config = Configuration()

config.section_('General')
config.General.transferOutputs = True
config.General.requestName = 'SingleEl_Run2012C_part2_v1'

config.section_('JobType')
config.JobType.psetName = 'semileptonic.py'
config.JobType.pluginName = 'Analysis'
config.JobType.inputFiles = ['../JEC/START53_V27_L1FastJet_AK7PFchs.txt', '../JEC/START53_V27_L2Relative_AK7PFchs.txt', '../JEC/START53_V27_L3Absolute_AK7PFchs.txt', '../JEC/START53_V27_Uncertainty_AK7PFchs.txt',
			     '../JEC/START53_V27_L1FastJet_AK5PFchs.txt', '../JEC/START53_V27_L2Relative_AK5PFchs.txt', '../JEC/START53_V27_L3Absolute_AK5PFchs.txt', '../JEC/START53_V27_Uncertainty_AK5PFchs.txt',
			     '../JEC/Winter14_V5_DATA_L1FastJet_AK7PFchs.txt', '../JEC/Winter14_V5_DATA_L2Relative_AK7PFchs.txt', '../JEC/Winter14_V5_DATA_L3Absolute_AK7PFchs.txt',
			     '../JEC/Winter14_V5_DATA_L2L3Residual_AK7PFchs.txt', '../JEC/Winter14_V5_DATA_Uncertainty_AK7PFchs.txt',
			     '../JEC/Winter14_V5_DATA_L1FastJet_AK5PFchs.txt', '../JEC/Winter14_V5_DATA_L2Relative_AK5PFchs.txt', '../JEC/Winter14_V5_DATA_L3Absolute_AK5PFchs.txt',
			     '../JEC/Winter14_V5_DATA_L2L3Residual_AK5PFchs.txt', '../JEC/Winter14_V5_DATA_Uncertainty_AK5PFchs.txt']
config.JobType.pyCfgParams = ['runOnData=1', 'JES=nominal', 'JER=nominal', 'includePDF=0', 'runOnCrab=1']

config.section_('Data')
config.Data.inputDataset = '/SingleElectron/knash-Run2012C-22Jan2013-MissingLumi_take2-v1_TLBSM_53x_v3-b11da83ff7a298b6297fefd09b0b7f27/USER'
config.Data.inputDBS = 'phys03'
config.Data.unitsPerJob = 80
config.Data.splitting = 'LumiBased'
config.Data.lumiMask = '../Cert_190456-208686_8TeV_22Jan2013ReReco_Collisions12_JSON.txt'
#config.Data.publishDataName = 'SingleEl_Run2012C_part2_v1'

config.section_('User')
config.Data.outLFNDirBase = '/store/user/lfeng/ntuples/jhu_diffmo'

config.section_('Site')
config.Site.storageSite = 'T3_US_FNALLPC'
