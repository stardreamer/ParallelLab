__author__ = 'doctor'
import fileinput
import copy
from itertools import zip_longest

def get_index(list,elem):
    idx = None
    idx = list.index(elem)
    return idx

def chunks(lst, count):
    n = len(lst) // count
    return list(x for x in zip_longest(*[iter(lst)] * n))

def cutter(filename, numtimes):
    times = []
    print("Opening %s:" % filename)
    ff = fileinput.FileInput(filename)
    lasttime = None
    tempfile = None
    print("Looking through %s: matching times" % filename)
    for line in ff:
        buf = line.split(";", 1)
        if ff.filelineno() != 1:
            if int(buf[0]) not in times:
                times.append(int(buf[0]))
    ff.close()
    print("Finished with matching!\n Preparing to print!")
    #times = chunks(times, numtimes)
    files = ["output%d.csv" % i for i in range(len(times))]
    #pfiles = [open(files[i], "wb") for i in range(len(times))]
    print("Reopening %s:" % filename)
    ff = fileinput.FileInput(filename)
    print("Starting printing information to file. \n It'll take a while, go and drink some tea")
    for line in ff:
        if ff.filelineno() == 1:
            for tempfilename in files:
                tempfile=open(tempfilename, "wb")
                tempfile.write(bytes(line, "utf-8"))
                tempfile.close()
        else:
             buf = line.split(";", 1)
             bufT = int(buf[0])
             if lasttime != bufT:
                tempfile.close()
                tempfile=open(files[get_index(times, bufT)], "wb")
                tempfile.write(bytes(line, "utf-8"))
                lasttime = bufT
             else:
                 tempfile.write(bytes(line, "utf-8"))
    tempfile.close()


    print("Ended with printing!")
    return files