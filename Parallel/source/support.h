#ifndef SUPPORT_H
#define SUPPORT_H
#define SIMPLE_BREAK 0
#define ADDAPRIVE_BREAK 1
#include <math.h>
#include <mpi.h>
struct Border{
	int left,right,length;
};
typedef struct Border border;
void Build_derived_type(border* indata, MPI_Datatype* message_type_ptr);
void getBorder(border* borders,int L,int size,int mode);

#endif
