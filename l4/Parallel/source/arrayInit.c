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
void arrayInit(array* emptyArray,int rank,int len,int ProcNum,int *source, int mode){
	//Рассчитаем границы
	border slice=getNum(rank, len, ProcNum);
	//Вычисление числа элементов и получение последовательности
	emptyArray->length=slice.length;
	emptyArray->Arr=getData(&slice, source, mode);	
}
