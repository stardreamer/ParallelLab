__author__ = 'doctor'
import fileinput
import copy

def cutter(filename, numtimes):
    print("Writing in output0.csv...")
    fout = open("output0.csv", "wb")
    header = ""
    lastidx = 0
    ff = fileinput.FileInput(filename)
    i = 1
    counter = 1
    files = []
    files.append("output0.csv")
    for line in ff:
        if ff.filelineno() == 1:
            header = copy.copy(line)
            fout.write(bytes(line, "utf-8"))
        else:
            buf = line.split(";", 1)
            if int(buf[0]) == lastidx:
                fout.write(bytes(line, "utf-8"))
            else:
                i += 1
                if i == numtimes:
                    fout.close()
                    print("Writing in output%d.csv..." % counter)
                    fout = open("output%d.csv" % counter, "wb")
                    files.append("output%d.csv" % counter)
                    fout.write(bytes(header, "utf-8"))
                    counter += 1
                    i = 1
                lastidx = int(buf[0])
                fout.write(bytes(line, "utf-8"))
    fout.close()
    return files