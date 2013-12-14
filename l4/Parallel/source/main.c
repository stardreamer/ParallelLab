#include "core.h"


int main(int argc, char *argv[]){
	long long int len=argc>1?atoll(argv[1]):100; //Длина последовательность
	int seed=argc>2?atoll(argv[2]):-1; //Семя для генератора случайных чисел
	array myArray=ARRAY_INIT; //Массив с данными
	int ProcNum=0,rank=0;


	MPI_Init(&argc,&argv); //Необходимо для вызова MPI функций
	
	MPI_Comm_size(MPI_COMM_WORLD,&ProcNum);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	
	
	if(!isPowerOfTwo(ProcNum)){
		if(rank==0)
			fprintf(stderr,"\n Wrong proc num\n");
		MPI_Finalize();
		return 1;
	}
	
	//Инициализация массива
	if(seed!=-1)
		arrayInit(&myArray,rank,len,ProcNum,&seed,DEFINED_RANDOM_MODE);
	else
		arrayInit(&myArray,rank,len,ProcNum,&seed,RANDOM_MODE);
	
	
	double t=core(&myArray, MY_MPI_QSORT);
	
	fprintf(stderr,"\nTime %lf\n",t);
	
	arrayFree(&myArray);
	MPI_Finalize();
	return 0;
}
