#include <stdio.h>
#include "support.h"

#define seed 1 //семя для генерации чисел
#define fMin -1.//нижняя граница чисел
#define fMax 1.//верхняя граница чисел



double fRand(){
    double f=(double)rand()/RAND_MAX;
    return fMin+f*(fMax-fMin);
}

int main(int argc, char** argv){
	double *a,*b,*c; //тут храним матрицы
	a=b=c=NULL;//устанавливаем указатели в NULL(важно, чтобы не происходила реаллокация неизвестно чего)
	double *times;//указатель на время работы(массив на root на остальных память только под один элемент)
	times=NULL;//устанавливаем указатель в NULL(важно, чтобы не происходила реаллокация неизвестно чего)
	border *borders; //границы разбиений
	borders=NULL;//устанавливаем указатель в NULL(важно, чтобы не происходила реаллокация неизвестно чего)
	int  N=0/*Число строк*/,L=0/*Число столбцов*/,N1/*Размерность a*/,N2/*Размерность b*/,N3/*Размерность c*/;
	double sum=0./*Сумма квадратов*/,resultnorm=0./*Норма*/;
	int rank/*id процесса*/,size/*Число процессов*/;
	MPI_Datatype message_type/*Тут будет храниться представление структуры border*/;
	
	
	L=argc>1?atoi(argv[1]):10;//Получаем L
	//Устанавливаем размерности согласно заданию
	N=10*L;
	N1=N*L;
	N2=L*L;
	N3=N*L;
	
	MPI_Init(&argc,&argv); //Необходимо для вызова MPI функций
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	
	//TODO: Во всех процессах счетчик инициализируется одним числом.(b заполняется во всех независимо) Исправить b похожий на a
	srand(seed);//Инициализируем генератор случайных чисел 
	
	b=(double*)malloc(sizeof(double)*N2);//Получаем указатель на матрицу b
	for(int i=0;i<N2;++i)	b[i]=fRand();//Заполняем матрицы псевдослучайными числами
	borders=(border*)malloc(sizeof(border));//Выделяем память под границы
	times=(double*)malloc(sizeof(double));//Выделяем память под один элемент
	Build_derived_type(borders,&message_type);//Инициализируем свой mpi тип
	
	
	//Нулевой процесс заполняет матрицу
	if(rank == 0){
		a=(double*)malloc(sizeof(double)*N1);//Выделяем память
		c=(double*)malloc(sizeof(double)*N3);//Получаем указатель на матрицу с
		borders=(border*)realloc(borders,sizeof(border)*size);
		for(int i=0;i<N1;++i)	a[i]=fRand();//Заполняем матрицы псевдослучайными числами
		getBorder(borders,N,size,SIMPLE_BREAK,NULL);
	}
	
	(*times)=Core_Candidat(a,b,c,borders,L,rank,size,&sum,&resultnorm,message_type); // тут происходит весь подсчет
	
	fprintf(stderr,"Rank %i [%i;%i] len %i result %lf time %lf(s)\n",rank,(*borders).left,(*borders).right,(*borders).length,sum,*times);
	MPI_Barrier(MPI_COMM_WORLD);
	if(rank==0){
		resultnorm=sqrt(resultnorm);
		fprintf(stderr,"Total result %lf time %lf(s)\n",resultnorm,(*times));
	}
	
	//Часть с неравномерным разбиением
	if(rank!=0){
		a=(double*)realloc(a,sizeof(double)*10*L);//Перевыделяем память для a
		for(int i=0;i<10*L;++i) a[i]=fRand();
		c=(double*)realloc(c,sizeof(double)*10*L);//Перевыделяем память для с
	}
	else{
		times=(double*)realloc(times,sizeof(double)*size);//на root перевыделяем память под времена
	}
	
	//Определяем производительность
	(*times)=-MPI_Wtime();
	Mprod(a,b,c,10,L,L);
	(*times) += MPI_Wtime();
	//Собираем времена на 0 процессе
	MPI_Gather(times,1,MPI_DOUBLE,times,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
	
	//Пересчитаем границы
	if(rank==0){
			getBorder(borders,N,size,ADDAPRIVE_BREAK,times);
	}
	
	MPI_Scatter(borders,1,message_type,borders,1,message_type,0,MPI_COMM_WORLD);
	
	fprintf(stderr,"rank %i border %i %i %i\n",rank,(*borders).left,(*borders).right,(*borders).length);
	(*times)=Core_Candidat(a,b,c,borders,L,rank,size,&sum,&resultnorm,message_type); // тут происходит весь подсчет
	
	fprintf(stderr,"Rank %i [%i;%i] len %i result %lf time %lf(s)\n",rank,(*borders).left,(*borders).right,(*borders).length,sum,*times);
	MPI_Barrier(MPI_COMM_WORLD);
	if(rank==0){
		resultnorm=sqrt(resultnorm);
		fprintf(stderr,"Total result %lf time %lf(s)\n",resultnorm,(*times));
	}
	
	MPI_Finalize();
	
	return 0;
}
