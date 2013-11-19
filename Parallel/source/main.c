#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "support.h"

#define seed 1
#define fMin -1.
#define fMax 1.
#define reductor(i,j,nu) (nu*i+j) 



double fRand(){
    double f=(double)rand()/RAND_MAX;
    return fMin+f*(fMax-fMin);
}

int main(int argc, char** argv){
	double *a,*b,*c; //тут храним матрицы
	border *borders; //границы разбиений
	int  N=0,L=0,N1,N2,N3;
	double sum=0.,abuf=0.,resultnorm=0.;
	int rank,size,*displs,*scounts;
	MPI_Status status;
	MPI_Datatype message_type;
	
	
	L=argc>1?atoi(argv[1]):10;
	N=10*L;
	N1=N*L;
	N2=L*L;
	N3=N*L;
	
	MPI_Init(&argc,&argv); //Необходимо для вызова MPI функций
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	srand(seed);//Инициализируем генератор случайных чисел
	
	b=(double*)malloc(sizeof(double)*N2);//Получаем указатель на матрицу b
	for(int i=0;i<N2;++i)	b[i]=fRand();//Заполняем матрицы псевдослучайными числами
	borders=(border*)malloc(sizeof(border));//Выделяем память под границы
	Build_derived_type(borders,&message_type);//Инициализируем свой mpi тип
	
	
	//Нулевой процесс заполняет матрицу
	if(rank == 0){
		a=(double*)malloc(sizeof(double)*N1);//Выделяем память
		c=(double*)malloc(sizeof(double)*N3);//Получаем указатель на матрицу с
		displs=(int*)malloc(sizeof(int)*size);
		scounts=(int*)malloc(sizeof(int)*size);
		borders=(border*)realloc(borders,sizeof(border)*size);
		for(int i=0;i<N1;++i)	a[i]=fRand();//Заполняем матрицы псевдослучайными числами
		getBorder(borders,N,size,SIMPLE_BREAK);
	}
	
	double wt=-MPI_Wtime();
	MPI_Scatter(borders,1,message_type,borders,1,message_type,0,MPI_COMM_WORLD);
	
	if(rank!=0){
		a=(double*)malloc(sizeof(double)*(*borders).length*L);//Выделяем память
		c=(double*)malloc(sizeof(double)*(*borders).length*L);//Получаем указатель на матрицу с
		for(int i=0;i<(*borders).length*L;i++) c[i]=0.;
	}
	else{
		for(int g=0;g<size;++g){//рассчитываем число элементов и смещение
			displs[g]=borders[g].left*L;
			scounts[g]=borders[g].length*L;
		}
	}
	
	for(int i=0;i<(*borders).length*L;++i) c[i]=0.;
		
	MPI_Scatterv(a,scounts,displs,MPI_DOUBLE,a,(*borders).length*L,MPI_DOUBLE,0,MPI_COMM_WORLD);
	//Перемножаем матрицы
	for(int i=0;i<(*borders).length;++i){
		for(int k=0;k<L;++k){
			abuf=a[reductor(i,k,L)]; //буфферизуем для ускорения доступа
			for(int j=0;j<L;++j)
				c[reductor(i,j,L)]+=abuf*b[reductor(k,j,L)];		
		}
	}
	
	
	//Считаем сумму квадратов
	for(int i=0;i<(*borders).length*L;++i)	sum+=c[i]*c[i];
	//Собираем сумму на root
	MPI_Reduce(&sum,&resultnorm,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
	wt += MPI_Wtime();//замер времени
	fprintf(stderr,"Rank %i [%i;%i] len %i result %lf time %lf(s)\n",rank,(*borders).left,(*borders).right,(*borders).length,sum,wt);
	MPI_Barrier(MPI_COMM_WORLD);
	if(rank==0){
		resultnorm=sqrt(resultnorm);
		fprintf(stderr,"Total result %lf time %lf(s)\n",resultnorm,wt);
	}
	
	MPI_Finalize();

	
	return 0;
}
