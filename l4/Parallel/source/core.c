#include "core.h"

/*
 * 
 * name: core
 * @param myArray указатель на массив
 * @param mode режим работы
 * @return t время работы
 * 
 */

double core(array* myArray, int mode){
	double t=0.;//Время
	
	int ProcNum=0,rank=0,color=0;
	ninja myNinja=NINJA_INIT;
	double lider=0.; //ведущий элемент
	MPI_Comm currentComm = MPI_COMM_WORLD;
	
	MPI_Comm_size(currentComm,&ProcNum);
	//Сортировка(пузырек)
	MyBubbleSort(myArray);
	
	//Начинаем замер времени
	t=MPI_Wtime();
	while(ProcNum!=1){

	MPI_Comm_rank(currentComm,&rank);
	fprintf(stderr,"~Pn %i \n", ProcNum);
	
	
	//Ведущий элемент
	lider=getAvg(myArray, currentComm);
	//Получаем ниндзя
	myNinja=getNinja(myArray,lider);
	MySwapAndMerge(myArray, &myNinja, rank, ProcNum, currentComm);
	
	color=(rank<ProcNum/2)?0:1;
	MPI_Comm_split(currentComm,color,rank,&currentComm);
	MPI_Comm_size(currentComm,&ProcNum);
	}
	
	t=MPI_Wtime()-t;
	
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	if(isSorted(myArray))
		myerror=SORT_SUCCESSED;
	else
		myerror=SORT_UNSUCCESSED;
	



	return t;
}
