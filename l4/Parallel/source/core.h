#ifndef CORE_H
#define CORE_H
#define MY_MPI_QSORT 1
#define MY_MPI_EVEN_N_EVEN 2
#define fMin -1000.//нижняя граница чисел
#define fMax 1000.//верхняя граница чисел
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include <sys/time.h>
#include "datatypes.h"


inline double fRand() __attribute__((always_inline));
inline int isPowerOfTwo (unsigned int x) __attribute__((always_inline));
inline double getSum(array* myArray) __attribute__((always_inline));

border getNum(int rank,long long int N,int size);
double* getData(border slice);
void MyBubbleSort(array* unsortedArray);
void arrayInit(array* emptyArray,int rank,long long int N,int size);
double core(array* myArray, int mode);


inline int isPowerOfTwo (unsigned int x){
	return ((x != 0) && ((x & (~x + 1)) == x));
}
inline double fRand(){
    double f=(double)rand()/RAND_MAX;
    return fMin+f*(fMax-fMin);
}

inline double getSum(array* myArray){
	double sum=0.;
	double *tempPointer=(*myArray).Arr;
	for(long long int i=0;i<(*myArray).myBorder.length;++i,tempPointer++)
		sum+=*tempPointer;
	return sum;	
}
#endif
