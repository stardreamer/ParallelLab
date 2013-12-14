#include "core.h"

/*
 * 
 * name: arrayInit
 * @param emptyArray указатель на пустой массив
 * @param rank  номер процесса
 * @param len длина последовательности
 * @param ProcNum общее число процессов
 * @param source указатель на источник данных
 * @param mode режим работы
 * @return VOID
 * 
 */
void arrayInit(array* emptyArray,int rank,long long int len,int ProcNum,int *source, int mode){
	//Вычисление числа элементов и получение последовательности
	(*emptyArray).myBorder=getNum(rank,len,ProcNum);
	(*emptyArray).Arr=getData((*emptyArray).myBorder,source,mode);	
}
