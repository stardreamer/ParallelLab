#include "core.h"

/*
 * 
 * name: core
 * @param myArray указатель на массив
 * @param mode режим работы
 * @return t время работы
 * 
 */

report core(array* myArray, int mode){
	double t=0.;//Время
	int rank=0;
	report localReport;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	
	//Начинаем замер времени
	t=MPI_Wtime();
	//Смотрим режим работы
	switch(mode){
		case MY_MPI_QSORT:
			localReport.errorCode=MyMpiQsort(myArray);
			localReport.mode="Quick Sort";
		break;
		
		case MY_MPI_EVEN_N_EVEN:
			localReport.errorCode=MyMpiEVESort(myArray);
			localReport.mode="Even not Even Sort";
		break;
		
		default:
			localReport.errorCode=UNKNOWN_MODE;
			localReport.mode="Unknown mode";
		break;
	}
	t=MPI_Wtime()-t;
	

	
	localReport.time=t;
	localReport.rank=rank;
	return localReport;
}
