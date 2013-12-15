#ifndef CORE_H
#define CORE_H
#define MY_MPI_QSORT 1
#define MY_MPI_EVEN_N_EVEN 2

#include "datatypes.h"
#include "myerrors.h"
#include <stdio.h>
#include <mpi.h>
#include <string.h>






inline int isPowerOfTwo (unsigned int x) __attribute__((always_inline));
inline double getAvg(array* myArray, MPI_Comm currentComm) __attribute__((always_inline));


report core(array* myArray, int mode); //вычислительное ядро
void MyQsortSwapAndMerge(array* myArray, ninja* myNinja, int rank, int ProcNum, MPI_Comm currentComm); //обмен и слияние данных
void MyEveSwapAndMerge(array* myArray, int rank, MPI_Comm currentComm); //обмен и слияние данных
void MyQsortMpiSwap(array* myArray, int ninjaIdx, int inlen, int outlen, int rank,int ProcNum, MPI_Comm currentComm); //обмен данными
int globalIsSorted(array* myArray, MPI_Comm currentComm);
int MyMpiQsort(array* myArray);
int MyMpiEVESort(array* myArray);

inline int isPowerOfTwo (unsigned int x){
	return ((x != 0) && ((x & (~x + 1)) == x));
}



inline double getAvg(array* myArray, MPI_Comm currentComm){
	double avg=0.;
	int Len=1;
	
	//Суммируем элементы в каждом массиве 
	avg=getSum(myArray);
	
	//Собраем число элементов в каждой группе и групповую сумму
	MPI_Allreduce(&(myArray->length), &Len, 1, MPI_INT, MPI_SUM, currentComm);
	MPI_Allreduce(&avg,&avg, 1, MPI_DOUBLE, MPI_SUM, currentComm);
	
	//Среднее арифметическое
	return (avg/(double)Len);
}
#endif
