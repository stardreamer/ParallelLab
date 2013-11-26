__author__ = 'doctor'
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.mlab import griddata
import os
print("Load data:")
data = np.genfromtxt('results.csv', dtype=[('TStep', int), ('x', float), ('y', float), ('U', float), ('dt', float)], comments='"', delimiter=';')
print("Preprocessing(sort):")
data.sort(axis=0)
TStep = data['TStep']
x = data['x']
y = data['y']
U = data['U']
files=[]
xbuf = x[TStep[:] == 0]
ybuf = y[TStep[:] == 0]

for time in range(TStep.max()+1):
    print("Creating frame: %03d"%time)
    fname = '_tmp%03d.png'%time
    Ubuf = U[TStep[:] == time]
    xi = np.linspace(xbuf.min(), xbuf.max(), len(set(xbuf)))
    yi = np.linspace(ybuf.min(), ybuf.max(), len(set(ybuf)))
    ui = griddata(xbuf, ybuf, Ubuf, xi, yi, interp='linear')
    plt.clf()
    plt.contourf(xi, yi, ui, 20, linewidth=1)
    plt.xlim(xbuf.min(), xbuf.max())
    plt.ylim(ybuf.min(), ybuf.max())
    plt.colorbar()
    plt.savefig(fname)
    files.append(fname)
print("Combining frames into video:")
os.system("mencoder 'mf://_tmp*.png' -mf type=png:fps=10 -ovc lavc -lavcopts vcodec=wmv2 -oac copy -o " + "HeatMap" + ".avi")
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


