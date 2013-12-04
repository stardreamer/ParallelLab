#ifndef CORE_H
#define CODE_H
#include "fileworker.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>
#include <time.h>
#include <math.h>

#define M_PI 3.14159265358979323846
#define N_K 0.00001864471911


struct Point{
	double X,Y,U;
};
struct Border{
	int left,right,length;
};

typedef struct Border border;
typedef struct Point point;

double core(double T,double L,int I,int NFRAMES);
void getBorder(border* borders,int N,int size);
void Build_derived_type(border* indata, MPI_Datatype* message_type_ptr);
void Build_derived_type_data(point* indata,MPI_Datatype* data_type_prt);

inline double lambda(double u) __attribute__((always_inline));
inline double rho(double u) __attribute__((always_inline));
inline double c(double u) __attribute__((always_inline));
inline double u0(double x,double y) __attribute__((always_inline));
inline double mu1(double y, double tau) __attribute__((always_inline));
inline double mu2(double y, double tau) __attribute__((always_inline));
inline double mu3(double x, double tau) __attribute__((always_inline));
inline double mu4(double x, double tau) __attribute__((always_inline));
inline int reductor(int i,int j,int l) __attribute__((always_inline));

void getBorder(border* borders,int N,int size);
void M_U(point *curPoint,point *lastPoint,int lx,int ly,int L,double dt,double T,double dx,double dy) ;



inline double c(double u){
	return 1./(2.25*1e-3-6.08*1e-10*u*u);
}

inline double u0(double x,double y){
	return (300.+400.*x)*(sin(M_PI*y)+1.);
}

inline double mu1(double y, double tau){
	return 300.+700.*tau/(y+1.);
}

inline double mu2(double y, double tau){
	return 300.*exp(-0.125*y*tau);
}

inline double mu3(double x, double tau){
	return 340.*x+(1.-x)*(300.+700.*tau);
}

inline double mu4(double x, double tau){
	return 340.*x*exp(-0.125*tau)+(1.-x)*(300.+350.*tau);
}

inline double lambda(double u){
	return 1.45+2.3*1e-2*u-2*1e-6*u*u;
}

inline double rho(double u){
	return 7860.+41500./u;
}

inline int reductor(int i,int j,int l){
	return (l*i+j);
}








#endif
