import glob
crab_cfgs = glob.glob('crab*.py')
txt_file = open('job_list.txt','w')
for line in crab_cfgs:
    txt_file.write(line+'\n')
txt_file.close()
