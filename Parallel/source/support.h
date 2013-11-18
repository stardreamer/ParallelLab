#ifndef SUPPORT_H
#define SUPPORT_H
#include <math.h>
#include <mpi.h>
struct Border{
	int left,right,length;
};
typedef struct Border border;
void Build_derived_type(border* indata, MPI_Datatype* message_type_ptr);



#endif
