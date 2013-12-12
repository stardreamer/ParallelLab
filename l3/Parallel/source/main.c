#include "core.h"


int main(int argc, char *argv[]){
	double L=1e-2;
	
	double T=argc>1?atof(argv[1]):1.;//Предельное время счёта
	int I=argc>2?atoi(argv[2]):100;//Число точек разбиения по каждой оси
	int NFRAMES=argc>3?atoi(argv[3]):100;//Число "кадров" выводимых в файл
	
	
	MPI_Init(&argc,&argv); //Необходимо для вызова MPI функций
	double t=core(T,L,I,NFRAMES);
	
	fprintf(stderr,"\nTime %lf\n",t);

	MPI_Finalize();
	return 0;
}
