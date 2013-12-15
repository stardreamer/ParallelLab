#include "core.h"

int myerror=0;

inline void printReport(report* myReport){
	fprintf(stderr,
		"Task: %s\n Result: %s\n Rank: %i\n Time: %lf\n",
		myReport->mode,
		errorString(myReport->errorCode),
		myReport->rank,
		myReport->time
		);
}

inline void printGlobalReport(report* myReport, int len){
	double minTime, maxTime;
	
	MPI_Reduce(&(myReport->time), &minTime, 1,MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);
	MPI_Reduce(&(myReport->time), &maxTime, 1,MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
	if(myReport->rank==0)
		fprintf(stderr,
			"Task: %s\n Result: %s\n Rank: %i\n MinTime: %lf\n MaxTime: %lf \n Length: %i\n",
			myReport->mode,
			errorString(myReport->errorCode),
			myReport->rank,
			minTime,
			maxTime,
			len
			);

}

int main(int argc, char *argv[]){

	int len=argc>1?atoll(argv[1]):100; //Длина последовательность
	int seed=argc>2?atoll(argv[2]):-1; //Семя для генератора случайных чисел
	array myArray=ARRAY_INIT; //Массив с данными
	report qReport,eReport; //Отчет
	int ProcNum=0,rank=0;


	MPI_Init(&argc,&argv); //Необходимо для вызова MPI функций
	
	MPI_Comm_size(MPI_COMM_WORLD,&ProcNum);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	
	

	
	//Инициализация массива
	if(seed!=-1)
		arrayInit(&myArray,rank,len,ProcNum,&seed,DEFINED_RANDOM_MODE);
	else
		arrayInit(&myArray,rank,len,ProcNum,&seed,RANDOM_MODE);
	
	
	qReport=core(&myArray, MY_MPI_QSORT);
	printReport(&qReport);
	arrayFree(&myArray);
	
	if(seed!=-1)
		arrayInit(&myArray,rank,len,ProcNum,&seed,DEFINED_RANDOM_MODE);
	else
		arrayInit(&myArray,rank,len,ProcNum,&seed,RANDOM_MODE);

	eReport=core(&myArray, MY_MPI_EVEN_N_EVEN);
	printReport(&eReport);
	arrayFree(&myArray);
	
	
	MPI_Barrier(MPI_COMM_WORLD);
	printGlobalReport(&qReport, len);
	printGlobalReport(&eReport, len);

	MPI_Finalize();
	return 0;
}
