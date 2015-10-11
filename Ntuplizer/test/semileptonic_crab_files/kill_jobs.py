import os
joblist = [line.rstrip('\n') for line in open('kill_list.txt') ]
for job in joblist:
    os.system('crab kill '+ job)
