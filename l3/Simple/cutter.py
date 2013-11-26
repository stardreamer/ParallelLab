__author__ = 'doctor'
import fileinput
import copy

fout = open("output0.csv", "wb")
header=""
lastidx=0
ff = fileinput.FileInput("results.csv")
i=1;
for line in ff:
    if ff.filelineno() == 1:
        header=copy.copy(line)
        fout.write(bytes(line, "utf-8"))
    else:
        buf = line.split(";", 1)
        if int(buf[0])%10!=0 or int(buf[0])==lastidx:
            fout.write(bytes(line, "utf-8"))
        else:
            i+=1
            fout.close()
            fout = open("output%d.csv"%i, "wb")
            fout.write(bytes(header, "utf-8"))
            fout.write(bytes(line, "utf-8"))
            lastidx=int(buf[0])
fout.close()