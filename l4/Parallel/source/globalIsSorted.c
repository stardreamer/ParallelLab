#include "core.h"

/**
 * 
 * Глобальная проверка на отсортированность массива разбросанного по процессам
 * @param myArray указатель на массив
 * @param currentComm коммуникатор для которого проверяется упорядоченность
 * @author Arthur Asylgareev (Virid Raven)
 * @return результат сортировки
 * 
 */
int globalIsSorted(array* myArray,MPI_Comm currentComm){
	int ProcNum=0/*число процессов в группе*/,rank=0/*номер процесса*/;
	double guest=0.; /*элемент с которым происходит сравнение*/
	
	
	MPI_Comm_size(currentComm, &ProcNum);
	MPI_Comm_rank(currentComm, &rank);
	
	//Проверяем локальную отсортированность
	if(!isSorted(myArray))
		return LOCAL_SORT_UNSUCCESSED;
	
	//Отравляем последний элемент массива на последующий процесс
	if(rank<ProcNum-1)
		MPI_Send(&(myArray->Arr[myArray->length-1]), 1,MPI_DOUBLE,rank+1,0,currentComm);
	
	/*fprintf(stderr,"last %lf %i\n",myArray->Arr[myArray->length-1],rank);
	fprintf(stderr," 0 %lf %i\n",myArray->Arr[0],rank);
	fprintf(stderr," 1 %lf %i\n",myArray->Arr[1],rank);*/
	//Проверяем что этот элемент не превосходит первый элемент массива	
	if(rank>0){
		MPI_Recv(&guest,1,MPI_DOUBLE,rank-1,0,currentComm,MPI_STATUS_IGNORE);
		if(guest>myArray->Arr[0])
			return SORT_UNSUCCESSED;
	}
	
	//Если прошли через все заграждения - свобода!
	return SORT_SUCCESSED;
}
