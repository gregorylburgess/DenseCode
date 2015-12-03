import os    
import re
def getFileSizes(dirname):
    filepaths = []
    for basename in os.listdir(dirname):
        filename = os.path.join(dirname, basename)
        if os.path.isfile(filename):
            filepaths.append(filename)

    # Re-populate list with filename, size tuples
    for i in range(len(filepaths)):
        filepaths[i] = (filepaths[i], os.path.getsize(filepaths[i]))
        print (filepaths[i][0][2:] + "\t" + str(filepaths[i][1]))


def scrapeTimes(file, regex):
    rslt = list()
    name = ""
    time = ""
    i=0;
    for line in open(file):
        a=re.findall(regex,line) 
        if(a):
            if(i%2):
                time = a[0][i%2]
                tup = (name,time)
                rslt.append(tup)
            else:
                name = a[0][i%2]
        i= i+1
    return rslt


times = scrapeTimes("./ETDC/timesComp", r'\*\* File compressed: ([\w+.]+)  \*\*|Time \(USER\)--> ([\d\.]+)')
print(times)