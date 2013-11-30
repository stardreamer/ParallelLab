__author__ = 'doctor'
import fileinput
import copy
from itertools import zip_longest

def get_index(list,elem):
    idx = None
    for sublist in list:
        if elem in sublist:
            idx = list.index(sublist)
    return idx

def chunks(lst, count):
    n = len(lst) // count
    return list(x for x in zip_longest(*[iter(lst)] * n))

def cutter(filename, numtimes):
    times = []
    ff = fileinput.FileInput(filename)
    linenumbers = 1;
    print("Looking through %s: matching times" % filename)
    for line in ff:
        buf = line.split(";", 1)
        if ff.filelineno() != 1:
            if int(buf[0]) not in times:
                times.append(int(buf[0]))
        linenumbers += 1

    times = chunks(times, numtimes)
    files = ["output%d.csv" % i for i in range(len(times))]
    pfiles = [open(files[i], "wb") for i in range(len(times))]

    ff = fileinput.FileInput(filename)
    print("Starting printing information to file. \n It'll take a while, go and drink some tea")
    for line in ff:
        if ff.filelineno() == 1:
            for outfile in pfiles:
                outfile.write(bytes(line, "utf-8"))
        else:
             buf = line.split(";", 1)
             pfiles[get_index(times, int(buf[0]))].write(bytes(line, "utf-8"))

    for outfile in pfiles:
        outfile.close()
    print("Ended with printing!")
    return files