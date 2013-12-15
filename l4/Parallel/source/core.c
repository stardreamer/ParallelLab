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
	
	myerror=MyMpiQsort(myArray);
		
	t=MPI_Wtime()-t;
	
	return t;
}
