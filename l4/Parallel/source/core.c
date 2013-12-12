#include "core.h"

double core(array* myArray, int mode){
	double t=0.;//Время
	
	int ProcNum=0,rank=0,color=0;
	long long int Len=1.;
	double avg=0.;
	MPI_Comm currentComm = MPI_COMM_WORLD;
	
	MPI_Comm_size(currentComm,&ProcNum);
	MPI_Comm_rank(currentComm,&rank);
	//Начинаем замер времени
	t=MPI_Wtime();
	
	//Сортировка(пузырек)
	MyBubbleSort(myArray);
	
	//Суммируем элементы в каждом массиве 
	avg=getSum(myArray);
	
	//Собраем число элементов в каждой группе и групповую сумму
	MPI_Allreduce(&(*myArray).myBorder.length, &Len, 1, MPI_LONG_LONG_INT, MPI_SUM, currentComm);
	MPI_Allreduce(&avg,&avg, 1, MPI_DOUBLE, MPI_SUM, currentComm);
	
	//Среднее арифметическое
	avg/=(double)Len;
	
	fprintf(stderr,"~^ %lf %i\n", avg, rank);
	color=(rank<ProcNum/2)?0:1;
	MPI_Comm_split(currentComm,color,rank,&currentComm);
	
	
	t=MPI_Wtime()-t;
	


	return t;
}
