#include "core.h"

/*
 * 
 * name: core
 * @param myArray указатель на массив
 * @param mode режим работы
 * @return t время работы
 * 
 */

double core(array* myArray, int mode){
	double t=0.;//Время
	
	//Начинаем замер времени
	t=MPI_Wtime();
	//Смотрим режим работы
	switch(mode){
		case MY_MPI_QSORT:
			myerror=MyMpiQsort(myArray);
		break;
		
		case MY_MPI_EVEN_N_EVEN:
			myerror=MyMpiEVESort(myArray);
		break;
		
		default:
			myerror=UNKNOWN_MODE;
		break;
	}
	t=MPI_Wtime()-t;
	
	return t;
}
