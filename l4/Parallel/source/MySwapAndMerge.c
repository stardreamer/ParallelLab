#include "core.h"

void MySwapAndMerge(array* myArray, ninja* myNinja, int rank, int ProcNum, MPI_Comm currentComm){
	long long int outlen=0,inlen=0;
	array bufArray=ARRAY_INIT;
	
	
	if(rank<(ProcNum/2)){
		outlen=myNinja->ninjaBlade;
		MPI_Send(&outlen,1,MPI_LONG_LONG_INT,rank+(ProcNum/2),0,currentComm);
		MPI_Recv(&inlen,1,MPI_LONG_LONG_INT,rank+(ProcNum/2),0,currentComm,MPI_STATUS_IGNORE);
	}
	else{
		outlen=myNinja->ninjaIdx;
		MPI_Recv(&inlen,1,MPI_LONG_LONG_INT,rank-(ProcNum/2),0,currentComm,MPI_STATUS_IGNORE);
		MPI_Send(&outlen,1,MPI_LONG_LONG_INT,rank-(ProcNum/2),0,currentComm);
	}
	

	
fprintf(stderr,"~o(iLen %lld)-(oLen %lld) %i\n",inlen,outlen, rank);

	
	if(outlen!=0){
		if(rank<(ProcNum/2))
			MPI_Send(&(myArray->Arr[myNinja->ninjaIdx]),myNinja->ninjaBlade,MPI_DOUBLE,rank+(ProcNum/2),0,currentComm);
		else
			MPI_Send(myArray->Arr,myNinja->ninjaIdx,MPI_DOUBLE,rank-(ProcNum/2),0,currentComm);
			
		if(inlen==0){
			myArray->length=(myArray->length-myNinja->ninjaBlade);
			if(myArray->length==0){
				free(myArray->Arr);
				myArray->Arr=NULL;
			}
			else{
				myArray->Arr=(double *)realloc(myArray->Arr,myArray->length*sizeof(double));
			}
		}
	}
	
	if(inlen!=0){
		
		bufArray.length=inlen;
		myArray->Arr=(double *)realloc(myArray->Arr,(bufArray.length+myArray->length)*sizeof(double));
		
		bufArray.Arr=&(myArray->Arr[myArray->length]);
		
		
		
		
		if(rank<(ProcNum/2))
			MPI_Recv(bufArray.Arr,inlen,MPI_DOUBLE,rank+(ProcNum/2),0,currentComm,MPI_STATUS_IGNORE);
		else
			MPI_Recv(bufArray.Arr,inlen,MPI_DOUBLE,rank-(ProcNum/2),0,currentComm,MPI_STATUS_IGNORE);
		
		if(rank<(ProcNum/2))
			memcpy(&(myArray->Arr[myNinja->ninjaIdx]), bufArray.Arr, sizeof(double)*bufArray.length);
		else{
			memcpy(myArray->Arr, &(myArray->Arr[myNinja->ninjaIdx]), sizeof(double)*(myArray->length-outlen));
			memcpy(&(myArray->Arr[myArray->length-outlen]), bufArray.Arr, sizeof(double)*bufArray.length);
		}
		myArray->length=(inlen+myArray->length-outlen);
		myArray->Arr=(double *)realloc(myArray->Arr,myArray->length*sizeof(double));
	}
	
	
	
	
}
