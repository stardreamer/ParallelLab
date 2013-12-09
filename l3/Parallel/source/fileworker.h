#ifndef FILE_WORKER_H
#define FILE_WORKER_H
#define GNUPLOT_F 1
#define PYTHONPLOT_F 2
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


void openFile(FILE **,const char*,int,int);
void deleteFile(const char*);
void printTimeStamp(FILE *,int timestep,double x,double y,double u,double time);
void cinematicTimeStamp(FILE *,int timestep,double x,double y,double u,double time,int fraps);
void closeFile(FILE **);
#endif
