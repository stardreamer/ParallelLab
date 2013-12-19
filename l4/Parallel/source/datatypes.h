#ifndef DATA_TYPE_H
#define DATA_TYPE_H

#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include "myerrors.h"

#define ARRAY_INIT {0, NULL}
#define NINJA_INIT {0, 0}
#define REPORT_INIT {0, UNKNOWN_MODE,0,0,0,"Unknown mode"}
#define RANDOM_MODE 3
#define DEFINED_RANDOM_MODE 4
#define fMin -1000. //нижняя граница чисел
#define fMax 1000. //верхняя граница чисел


typedef struct Border border;
typedef struct Array array;
typedef struct Ninja ninja;
typedef struct Report report;

/**
 * 
 * Граница разбиений
 */
struct Border{ //левая правая граница длина
	int left; /**< Левая граница разбиения*/
	int right;/**< Правая граница разбиения*/
	int length;/**< Длина разбиения*/
};

/**
 * 
 * Массив, знающий свою длину 
 * 
 */
struct Array{ //размер + указатель на массив
	int length; /**< Длина массива*/
	double* Arr; /**< Указатель на массив*/
};

/**
 * 
 * Структура хранящая индекс первого элемента, большего чем ведущий 
 */
struct Ninja{ //В структуре хранится индекс первого элемента массива больше ведущего и указатель на него
	int ninjaIdx; /**< Индекс элемента*/
	int ninjaBlade; /**< Длина отрезка от элемента до конца массива*/
};

/**
 * 
 * Структура-отчет
 */
struct Report{ //В структуре хранится отчет о работе
	double time; /**< Время работы*/
	int errorCode; /**< Код ошибки*/
	int rank; /**< Номер процесса*/
	int steps; /**< Число шагов алгоритма*/
	int len; /**< Длина массива на процессе*/
	const char* mode; /**< Режим работы*/
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
int MyFastNormalizator(array* preSorted,array* pastArray);

int   getNinjaIdx(array* wholeArray, double lider);
ninja getNinja(array* wholeArray, double lider);

/**
 * 
 * Генерирует случайное число в гарницах от fMin до fMax
 * @return  случайное число
 * 
 */

inline double fRand(){
    double f=(double)rand()/RAND_MAX;
    return fMin+f*(fMax-fMin);
}

/**
 * 
 * Проверят отсортирован ли массив
 * @param misticArray указатель на массив
 * @return результат проверки
 * @author Arthur Asylgareev (Virid Raven)
 * @see #array
 */

inline int isSorted(array* misticArray){
	int i=0;
	for(; i<(misticArray->length-1);i++)
		if(misticArray->Arr[i]>misticArray->Arr[i+1])
			break;
	return i==misticArray->length-1;
}

/**
 * 
 * Сумма всех элементов
 * @param misticArray указатель на массив
 * @return  сумма всех элементов массива 
 * @author Arthur Asylgareev (Virid Raven)
 * @see #array
 */
inline double getSum(array* myArray){
	double sum=0.;
	double *tempPointer=myArray->Arr;
	for(int i=0;i<myArray->length;++i,tempPointer++)
		sum+=*tempPointer;
	return sum;	
}

#endif
