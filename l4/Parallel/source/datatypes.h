#ifndef DATA_TYPE_H
#define DATA_TYPE_H
#define ARRAY_INIT {0, NULL}
#define NINJA_INIT {0, 0}
#define RANDOM_MODE 3
#define DEFINED_RANDOM_MODE 4
#define fMin -1000. //нижняя граница чисел
#define fMax 1000. //верхняя граница чисел
#define TRUE (42==42)
#define FALSE (!TRUE)

#include <time.h>
#include <sys/time.h>
#include <stdlib.h>

typedef struct Border border;
typedef struct Array array;
typedef struct Ninja ninja;
typedef struct Report report;

struct Border{ //левая правая граница длина
	int left,right,length;
};

struct Array{ //размер + указатель на массив
	int length;
	double* Arr;
};


struct Ninja{ //В структуре хранится индекс первого элемента массива больше ведущего и указатель на него
	int ninjaIdx, ninjaBlade;
};

struct Report{ //В структуре хранится отчет о работе
	double time;
	int errorCode;
	int rank;
	const char* mode;
};

inline double fRand() __attribute__((always_inline));
inline int isSorted(array* misticArray) __attribute__((always_inline));
inline double getSum(array* myArray) __attribute__((always_inline));


border getNum(int rank,int N,int size); //получение границ
double* getData(border* slice,void *source, int mode); //получение данных
int MyBubbleSort(array* unsortedArray); //пузырьковая сортировка
int MyNormalizator(array* firstArray); //упорядочиватель 
void arrayInit(array* emptyArray,int rank,int N,int size, int *seed, int mode); //инициаллизация массива
void arrayFree(array* emptyArray); //освобождение памяти


int   getNinjaIdx(array* wholeArray, double lider);
ninja getNinja(array* wholeArray, double lider);

inline double fRand(){
    double f=(double)rand()/RAND_MAX;
    return fMin+f*(fMax-fMin);
}

inline int isSorted(array* misticArray){
	int i=0;
	for(; i<(misticArray->length-1);i++)
		if(misticArray->Arr[i]>misticArray->Arr[i+1])
			break;
	return i==misticArray->length-1;
}

inline double getSum(array* myArray){
	double sum=0.;
	double *tempPointer=myArray->Arr;
	for(int i=0;i<myArray->length;++i,tempPointer++)
		sum+=*tempPointer;
	return sum;	
}

#endif
