#ifndef FILE_WORKER_H
#define FILE_WORKER_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void openFile(FILE **,const char*,int);
void printTimeStamp(FILE *,int timestep,double x,double y,double u,double time);
void cinematicTimeStamp(FILE *,int timestep,double x,double y,double u,double time,int fraps);
void closeFile(FILE **);
#endif
