# You can run pTestMaker by giving one argument(number of processes).
# And two other optional arguments the range of runtime and priority.

# python pTestMaker 10 > testfile
# creates a test file with 10 processes
# python pTestMaker 10 40 > testfile
# creates a test file with 10 processes with 
# 	random runtime range from 1 to 40

# python pTestMaker 10 40 3 > testfile
# creates a test file with 10 processes with 
# 	random runtime range from 1 to 40
#   and
# 	random priority range from 1 to 4
import sys
from random import randrange 

argc = len(sys.argv)
pRange = int(sys.argv[1]) if(argc > 1) else 10;
rRange = int(sys.argv[2]) + 1 if(argc > 2)  else 50;
priRange = int(sys.argv[3]) + 1 if(argc > 3)  else 1;

for x in range(0, pRange):
	arrivalTime = randrange(0, x+1);
	runTime = randrange(1, rRange);
	priority = randrange(0, priRange);
	print("P{0} {1} {2} {3}".format(x, arrivalTime, runTime, priority));