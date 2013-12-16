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
	myerror=1;
	report localReport;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	localReport.steps=0;
	//Начинаем замер времени
	t=MPI_Wtime();
	//Смотрим режим работы
	switch(mode){
		case MY_MPI_QSORT:
			localReport.steps=MyMpiQsort(myArray);
			localReport.mode="Quick Sort";
		break;
		
		case MY_MPI_EVEN_N_EVEN:
			localReport.steps=MyMpiEVESort(myArray);
			localReport.mode="Even not Even Sort";
		break;
		
		default:
			localReport.errorCode=UNKNOWN_MODE;
			localReport.mode="Unknown mode";
		break;
	}
	t=MPI_Wtime()-t;
	
	if(myerror==1)
		myerror=globalIsSorted(myArray,MPI_COMM_WORLD);
		
	localReport.errorCode=myerror;
	localReport.len=myArray->length;
	localReport.time=t;
	localReport.rank=rank;
	return localReport;
}
