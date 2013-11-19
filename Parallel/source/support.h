#ifndef SUPPORT_H
#define SUPPORT_H
#define SIMPLE_BREAK 0
#define ADDAPRIVE_BREAK 1
#define reductor(i,j,nu) (nu*i+j) 

#include <math.h>
#include <mpi.h>
struct Border{
	int left,right,length;
};
typedef struct Border border;
void Build_derived_type(border* indata, MPI_Datatype* message_type_ptr);
void getBorder(border* borders,int N,int size,int mode,double *);
void Mprod(double *a,double *b,double *c,int l1,int l2,int l3);
double Core_Candidat();
#endif
