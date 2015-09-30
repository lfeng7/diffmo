from WMCore.Configuration import Configuration
config = Configuration()

config.section_('General')
config.General.transferOutputs = True
config.General.requestName = 'TTBar_Powheg_tester_noPDF'

config.section_('JobType')
config.JobType.psetName = 'semileptonic.py'
config.JobType.pluginName = 'Analysis'
config.JobType.inputFiles = ['../JEC/START53_V27_L1FastJet_AK7PFchs.txt', '../JEC/START53_V27_L2Relative_AK7PFchs.txt', '../JEC/START53_V27_L3Absolute_AK7PFchs.txt', '../JEC/START53_V27_Uncertainty_AK7PFchs.txt',
								'../JEC/START53_V27_L1FastJet_AK5PFchs.txt', '../JEC/START53_V27_L2Relative_AK5PFchs.txt', '../JEC/START53_V27_L3Absolute_AK5PFchs.txt', '../JEC/START53_V27_Uncertainty_AK5PFchs.txt',
								'../JEC/Winter14_V5_DATA_L1FastJet_AK7PFchs.txt', '../JEC/Winter14_V5_DATA_L2Relative_AK7PFchs.txt', '../JEC/Winter14_V5_DATA_L3Absolute_AK7PFchs.txt',
								'../JEC/Winter14_V5_DATA_L2L3Residual_AK7PFchs.txt', '../JEC/Winter14_V5_DATA_Uncertainty_AK7PFchs.txt',
								'../JEC/Winter14_V5_DATA_L1FastJet_AK5PFchs.txt', '../JEC/Winter14_V5_DATA_L2Relative_AK5PFchs.txt', '../JEC/Winter14_V5_DATA_L3Absolute_AK5PFchs.txt',
								'../JEC/Winter14_V5_DATA_L2L3Residual_AK5PFchs.txt', '../JEC/Winter14_V5_DATA_Uncertainty_AK5PFchs.txt']
config.JobType.pyCfgParams = ['runOnData=0', 'JES=nominal', 'JER=nominal', 'includePDF=0', 'runOnCrab=1']

config.section_('Data')
config.Data.inputDataset = '/TT_CT10_TuneZ2star_8TeV-powheg-tauola/StoreResults-Summer12_DR53X-PU_S10_START53_V7A-v2_TLBSM_53x_v3_bugfix_v1-99bd99199697666ff01397dad5652e9e/USER'
config.Data.splitting = 'LumiBased'
config.Data.unitsPerJob = 1
config.Data.totalUnits = 5

config.Data.publication = False 
#config.Data.publishDataName = 'TTBar_Powheg_tester'

config.section_('User')

config.Data.outLFNDirBase = '/store/user/lfeng/ntuples/jhu_diffmo'

config.section_('Site')
config.Site.storageSite = 'T3_US_FNALLPC'
