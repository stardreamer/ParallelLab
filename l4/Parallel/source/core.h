#ifndef CORE_H
#define CORE_H
#define MY_MPI_QSORT 1
#define MY_MPI_EVEN_N_EVEN 2

#include "datatypes.h"
#include <stdio.h>
#include <mpi.h>





inline int isPowerOfTwo (unsigned int x) __attribute__((always_inline));
inline double getSum(array* myArray) __attribute__((always_inline));
inline double getAvg(array* myArray, MPI_Comm currentComm) __attribute__((always_inline));


double core(array* myArray, int mode);
void MyMerge(array* myArray, ninja* myNinja, int rank, int ProcNum, MPI_Comm currentComm);

inline int isPowerOfTwo (unsigned int x){
	return ((x != 0) && ((x & (~x + 1)) == x));
}

inline double getSum(array* myArray){
	double sum=0.;
	double *tempPointer=(*myArray).Arr;
	for(long long int i=0;i<(*myArray).myBorder.length;++i,tempPointer++)
		sum+=*tempPointer;
	return sum;	
}

inline double getAvg(array* myArray, MPI_Comm currentComm){
	double avg=0.;
	long long int Len=1;
	
	//Суммируем элементы в каждом массиве 
	avg=getSum(myArray);
	
	//Собраем число элементов в каждой группе и групповую сумму
	MPI_Allreduce(&(*myArray).myBorder.length, &Len, 1, MPI_LONG_LONG_INT, MPI_SUM, currentComm);
	MPI_Allreduce(&avg,&avg, 1, MPI_DOUBLE, MPI_SUM, currentComm);
	
	//Среднее арифметическое
	return (avg/(double)Len);
}
#endif