#include "core.h"

void MyMerge(array* myArray, ninja myNinja, int rank, int ProcNum, MPI_Comm currentComm){
	long long int outlen=myNinja.ninjaBlade,inlen=0;
	
	if(rank<(ProcNum/2)){
		MPI_Send(&outlen,1,MPI_LONG_LONG_INT,rank+(ProcNum/2),0,currentComm);
		MPI_Recv(&inlen,1,MPI_LONG_LONG_INT,rank+(ProcNum/2),0,currentComm,MPI_STATUS_IGNORE);
	}
	else{
		MPI_Recv(&inlen,1,MPI_LONG_LONG_INT,rank-(ProcNum/2),0,currentComm,MPI_STATUS_IGNORE);
		MPI_Send(&outlen,1,MPI_LONG_LONG_INT,rank-(ProcNum/2),0,currentComm);
	}
}
