#include "core.h"

/*
 * 
 * name: MySwapAndMerge
 * @param myArray указатель на массив
 * @param myNinja указатель на ниндзю
 * @param rank номер процесса
 * @param ProcNum общее число процессов
 * @param currentComm  коммуникатор
 * @return void
 * 
 */
void MySwapAndMerge(array* myArray, ninja* myNinja, int rank, int ProcNum, MPI_Comm currentComm){
	long long int outlen=0,inlen=0; // число принимаемых и отправляемых элементов
	
	
	if(rank<(ProcNum/2)){ // отправляем хвост
		outlen=myNinja->ninjaBlade;
		MPI_Send(&outlen,1,MPI_LONG_LONG_INT,rank+(ProcNum/2),0,currentComm);
		MPI_Recv(&inlen,1,MPI_LONG_LONG_INT,rank+(ProcNum/2),0,currentComm,MPI_STATUS_IGNORE);
	}
	else{ // отправляем голову 
		outlen=myNinja->ninjaIdx;
		MPI_Recv(&inlen,1,MPI_LONG_LONG_INT,rank-(ProcNum/2),0,currentComm,MPI_STATUS_IGNORE);
		MPI_Send(&outlen,1,MPI_LONG_LONG_INT,rank-(ProcNum/2),0,currentComm);
	}
	

	
	fprintf(stderr,"~o(iLen %lld)-(oLen %lld) %i\n",inlen,outlen, rank);

	//Обмениваемся элементами
	MyMpiSwap(myArray, myNinja->ninjaIdx, inlen, outlen, rank, ProcNum, currentComm);
	//Нормализуем массив
	MyNormalizator(myArray);
	
	
	
}
