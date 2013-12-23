#include "core.h"


int main(int argc, char *argv[]){
	double L=1e-2;
	double maxTime=0.;
	int rank=0,ProcNum=0;
	double T=argc>1?atof(argv[1]):1.;//Предельное время счёта
	int I=argc>2?atoi(argv[2]):100;//Число точек разбиения по каждой оси
	int NFRAMES=argc>3?atoi(argv[3]):100;//Число "кадров" выводимых в файл
	
	
	MPI_Init(&argc,&argv); //Необходимо для вызова MPI функций
	double t=core(T,L,I,NFRAMES);
	
	MPI_Comm_size(MPI_COMM_WORLD,&ProcNum);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Reduce(&(t), &maxTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
	if(rank==0)
		fprintf(stderr,"\nTime %lf  proc %i\n",maxTime,ProcNum);

	MPI_Finalize();
	return 0;
}
