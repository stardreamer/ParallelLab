__author__ = 'doctor'
from cutter import cutter
from animatornew import animatorFunc
import os


filelist = cutter("results.csv", 10)
animatorFunc(filelist, "HeatMap")
os.system("rm -f output* Heat.gif")
os.system("ffmpeg -i HeatMap.avi Heat.gif")