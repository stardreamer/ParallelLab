#include "core.h"

void arrayInit(array* emptyArray,int rank,long long int len,int ProcNum){
	//Вычисление числа элементов и получение последовательности
	(*emptyArray).myBorder=getNum(rank,len,ProcNum);
	(*emptyArray).Arr=getData((*emptyArray).myBorder);	
}
