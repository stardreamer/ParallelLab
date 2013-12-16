#include "core.h"

/**
 * 
 * Вычислительное ядро. В зависимости от режима производит
 * различные сортировки
 * @param myArray указатель на массив
 * @param mode режим работы
 * @author Arthur Asylgareev (Virid Raven)
 * @return  время работы
 * 
 */

report core(array* myArray, int mode){
	double t=0.;//Время
	int rank=0;
	myerror=1;
	report localReport=REPORT_INIT;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	
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
