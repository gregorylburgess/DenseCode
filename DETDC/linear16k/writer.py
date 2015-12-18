import sys



def writeData(numWords, numReps, filename):
	s= ""
	i=1
	while (i<=numWords):
		s += str(i) + " "
		i=i+1
	f = open(filename, 'w')
	i=1
	while (i<=numReps):
		f.write(s)
		i=i+1
	f.close()
	return
if(len(sys.argv)<4):
  sys.exit()
print(sys.argv)
numWords = int(sys.argv[1])
numReps = int(sys.argv[2])
name = sys.argv[3]
writeData(numWords,numReps,name)
