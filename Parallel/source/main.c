#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "support.h"

#define seed 1
#define fMin -1.
#define fMax 1.
#define reductor(i,j,nu) (((i<nu) && (j<nu)) ? (nu*i+j) : -1)



double fRand(){
    double f=(double)rand()/RAND_MAX;
    return fMin+f*(fMax-fMin);
}

int main(int argc, char** argv){
	double *a,*b,*c; //тут храним матрицы
	border *borders; //границы разбиений
	int *numOfLines, N=0,L=0,N1,N2,N3,MyIdx=0,h,m;
	double sum=0.;
	int rank,size;
	MPI_Status status;
	MPI_Datatype message_type;
	L=argc>1?atoi(argv[1]):10;
	N=10*L;
	N1=N*L;
	N2=L*L;
	N3=N*L;
	
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	b=(double*)malloc(sizeof(double)*N2);//Получаем указатель на матрицу b
	for(int i=0;i<N2;++i)	b[i]=fRand();//Заполняем матрицы псевдослучайными числами
	borders=(border*)malloc(sizeof(border));//Выделяем память под границы
	
	Build_derived_type(borders,&message_type);//Инициализируем свой mpi тип
	//Нулевой процесс заполняет матрицу
	if(rank == 0){
		a=(double*)malloc(sizeof(double)*N1);//Выделяем память
		c=(double*)malloc(sizeof(double)*N3);//Получаем указатель на матрицу с
		borders=(border*)realloc(borders,sizeof(border)*size);
		numOfLines=(int*)malloc(sizeof(int)*size);
		srand(seed);//Инициализируем генератор случайных чисел
		for(int i=0;i<N1;++i)	a[i]=fRand();//Заполняем матрицы псевдослучайными числами
		for(int i=0;i<size;++i)	numOfLines[i]=i;
		
		h=L/(size);
		m=(L%(size));
	
		if(m==0){
			for(int g=0;g<size;g++){
				borders[g].left=(g)*h;
				borders[g].right=(g+1)*h-1;
				borders[g].length=h;
			}
		}
		else{
			
				for(int g=0;g<m;++g){
					borders[g].left=g*h;
					borders[g].right=(g+1)*h;
					borders[g].length=h+1;
				}
				
				for(int g=m;g<size;++g){
					borders[g].left=g*h+1;
					borders[g].right=(g+1)*h;
					borders[g].length=h;
				}		
		}
	}
	
	MPI_Scatter(borders,1,message_type,borders,1,message_type,0,MPI_COMM_WORLD);
	
	MPI_Barrier(MPI_COMM_WORLD);
		/*a=(int*)malloc(sizeof(int)*N1);
		b=(int*)malloc(sizeof(int)*N2);//Получаем указатель на матрицу b
		c=(int*)malloc(sizeof(int)*N3);//Получаем указатель на матрицу с
		//Перемножаем матрицы
		for(int i=0;i<n;++i){
			for(int k=0;k<n;k++){
				abuf=a[reductor(i,k,n)];
				for(int j=0;j<n;++j){
					c[reductor(i,j,n)]+=abuf*b[reductor(k,j,n)];
				}
			}
		}*/
	fprintf(stderr,"Rank %i MyBorder=%i %i %i\n",rank,(*borders).left,(*borders).right,(*borders).length);
	
	
	MPI_Finalize();
	return 0;
}
