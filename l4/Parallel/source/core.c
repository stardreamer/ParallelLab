#include "core.h"

double core(array* myArray, int mode){
	double t=0.;//Время
	
	int ProcNum=0,rank=0,color=0;
	ninja myNinja=NINJA_INIT;
	double lider=0.; //ведущий элемент
	MPI_Comm currentComm = MPI_COMM_WORLD;
	
	MPI_Comm_size(currentComm,&ProcNum);
	MPI_Comm_rank(currentComm,&rank);
	
	//Начинаем замер времени
	t=MPI_Wtime();
	
	//Сортировка(пузырек)
	MyBubbleSort(myArray);
	
	//Ведущий элемент
	lider=getAvg(myArray, currentComm);
	//Получаем ниндзя
	myNinja=getNinja(myArray,lider);
	
	fprintf(stderr,"~^ %lf %i\n", lider, rank);
	for(long long int i=0;i<(*myArray).length;++i)
		fprintf(stderr,"~e %lf %i\n", (*myArray).Arr[i], rank);
	
	fprintf(stderr,"~i %i %i(Len %lld)\n", myNinja.ninjaIdx, rank, (*myArray).length);
	MPI_Barrier(currentComm);

	MySwapAndMerge(myArray, &myNinja, rank, ProcNum, currentComm);
	MPI_Barrier(currentComm);
	fprintf(stderr,"MYRGE\n");
	for(long long int i=0;i<(*myArray).length;++i)
		fprintf(stderr,"~e %lf %i\n", (*myArray).Arr[i], rank);
	MPI_Barrier(currentComm);
	//fprintf(stderr,"~o(iLen %lld)-(oLen %lld) %i\n",inlen,outlen, rank);
	fprintf(stderr,"~i %i %i(Len %lld)\n", myNinja.ninjaIdx, rank, (*myArray).length);
	color=(rank<ProcNum/2)?0:1;
	MPI_Comm_split(currentComm,color,rank,&currentComm);
	
	
	t=MPI_Wtime()-t;
	


	return t;
}
