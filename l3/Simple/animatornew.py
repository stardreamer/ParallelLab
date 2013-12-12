__author__ = 'doctor'
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.mlab import griddata
import os

def animatorFunc(filelist,moviename):
    counter = 0
    fcounter = 0
    files = []
    for file in filelist:
        print("Load data from %s(%i/%i)..." % (filelist[fcounter], fcounter+1, len(filelist)))
        data = np.genfromtxt(file, dtype=[('TStep', int), ('x', float), ('y', float), ('U', float), ('dt', float)], comments='"', delimiter=';')
        TStep = data['TStep']
        x = data['x']
        y = data['y']
        U = data['U']
        dt = data['dt']
        xbuf = x[TStep[:] == TStep[0]]
        ybuf = y[TStep[:] == TStep[0]]
        print("Generating frames %s(%i/%i)..." % (filelist[fcounter], fcounter+1, len(filelist)))
        for time in sorted(set(TStep)):
            fname = '_tmp%04d.png' % counter
            Ubuf = U[TStep[:] == time]
            #print(len(xbuf)," ", len(ybuf)," ",len(Ubuf))
            xi = np.linspace(xbuf.min(), xbuf.max(), len(set(xbuf)))
            yi = np.linspace(ybuf.min(), ybuf.max(), len(set(ybuf)))
            ui = griddata(xbuf, ybuf, Ubuf, xi, yi, interp='linear')
            if int(Ubuf.max()-Ubuf.min())<1000:
                lev = int(Ubuf.max()-Ubuf.min())
            else:
                lev = 1000
            plt.clf()
            #plt.contourf(xi, yi, ui, lev, vmin=100, vmax=1400, linewidth=1)
            plt.pcolormesh(xi, yi, ui, vmin=100, vmax=1400)
            plt.xlim(xbuf.min(), xbuf.max())
            plt.ylim(ybuf.min(), ybuf.max())
            curtime = dt[TStep[:] == time]
            plt.title("Time: %f" % curtime[0], ha="center")
            plt.colorbar()
            plt.savefig(fname)
            files.append(fname)
            counter += 1
        fcounter += 1
    print("Combining frames into video:")
    os.system("mencoder 'mf://_tmp*.png' -mf type=png:fps=30 -ovc lavc -lavcopts vcodec=wmv2 -oac copy -o " + moviename + ".avi")
    # cleanup
    for fname in files: os.remove(fname)

#xi = np.linspace(x.min(), x.max(), ngrid)
#yi = np.linspace(y.min(), y.max(), ngrid)
#ui = griddata(x, y, U, xi, yi)
#
#plt.contourf(xi, yi, ui, 20, linewidth=1)
##plt.scatter(x, y, c=U, s=20)
#
#plt.xlim(x.min(), x.max())
#plt.ylim(y.min(), y.max())
#plt.colorbar()
#plt.show()


