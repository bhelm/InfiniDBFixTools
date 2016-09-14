# InfiniDB Fix tools to recover from critical crashes
the tools in this toolkit are not well tested, so use them at your own risk. better have a backup!
this toolkit contains classes and files from the InfiniDB github: https://github.com/infinidb/infinidb

## vbbmfix
recovers from DBRM Failed errors caused by "VBBM::load(): Failed to load entry" errors.
you can find the load_brm command on the debug log to find out which dbrm save is loaded and try this command manually.
then you will see the VBBM::load(): Failed to load entry error. this tool works by reading and validating the vbbm 
file format and adjusting the entry count so infinidb stops complaining. i dont know how this corruption occures and
if the file is 100% correct afterwards. it is meant as a last-hope rescue tool, as it may cause unpredictable behavior, 
so use on your own risk. after usage, verify that all tables are working and contain the correct data.
for me, it worked fine.

## lockread
allows the view the infinidb lockfile "data1/systemFiles/dbrm/tablelocks" while infinidb is offline, so you can verify
all the locking processes are gone. after you confirmend this, the tablelocks file can be deleted.