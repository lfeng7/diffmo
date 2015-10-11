import os
joblist = [line.rstrip('\n') for line in open('resubmit_list.txt') ]
for job in joblist:
    os.system('crab resubmit '+ job)
