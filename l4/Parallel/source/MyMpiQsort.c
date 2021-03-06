#include "core.h"

/**
 * 
 * Организует параллельную "быструю" сортировку массива 
 * @param myArray указатель на массив
 * @author Arthur Asylgareev (Virid Raven)
 * @return число шагов
 * 
 */

int MyMpiQsort(array* myArray){
	int ProcNum=0,rank=0,color=0,count=0;
	ninja myNinja=NINJA_INIT;
	double lider=0.; //ведущий элемент
	MPI_Comm currentComm = MPI_COMM_WORLD;
	
	MPI_Comm_size(currentComm, &ProcNum);
	
	if(!isPowerOfTwo(ProcNum)){
		myerror=WRONG_PROC_NUMBER;
		return 0;
	}
	
	//Сортировка(пузырек)
	MyBubbleSort(myArray);
	
	//Сортировка(MPI_QSORT)
	while(ProcNum!=1){
		++count;
		MPI_Comm_rank(currentComm,&rank);		
		
		//Ведущий элемент
		lider=getAvg(myArray, currentComm);
		
		//Получаем ниндзя
		myNinja=getNinja(myArray,lider);
		
		//Обмениваемся данными и сливаем массивы
		MyQsortSwapAndMerge(myArray, &myNinja, rank, ProcNum, currentComm);
		
		//Помечаем процессы для разбиения на группы
		color=(rank<ProcNum/2)?0:1;
		MPI_Comm_split(currentComm,color,rank,&currentComm);
		MPI_Comm_size(currentComm,&ProcNum);
	}
	
	return count;
}
