import os
joblist = [line.rstrip('\n') for line in open('job_list.txt') ]
for job in joblist:
    os.system('crab submit '+ job)
