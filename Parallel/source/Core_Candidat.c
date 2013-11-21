#include "support.h"

double Core_Candidat(double *a,double *b,double *c,border *borders,int L,int rank,int size,double *sum,double *resultnorm,MPI_Datatype message_type){
	int *displs/*указатель на смещения*/,*scounts/*указатель на число элеемнтов*/;
	*sum=0.;/*Зануляем перед подсчетом*/
	double wt=-MPI_Wtime();/*Засекаем время*/
	displs=scounts=NULL;
	//Рассылаем границы разбиений(они идут друг за другом номера элементов соответствуют номерам процессов)
	MPI_Scatter(borders,1,message_type,borders,1,message_type,0,MPI_COMM_WORLD);
	
	if(rank!=0){
		if(a==NULL)	a=(double*)malloc(sizeof(double)*(*borders).length*L);//Выделяем память
		else        a=(double*)realloc(a,sizeof(double)*(*borders).length*L);//Если выделена, то перевыделяем
		if(c==NULL) c=(double*)malloc(sizeof(double)*(*borders).length*L);//Получаем указатель на матрицу с
		else        c=(double*)realloc(c,sizeof(double)*(*borders).length*L);//Если выделена, то перевыделяем
		if(a==NULL || c==NULL) MPI_Abort(MPI_COMM_WORLD,1);//возможно была ошибка у realloc
		for(int i=0;i<(*borders).length*L;i++) c[i]=0.;//зануляем с
	}
	else{
		displs=(int*)malloc(sizeof(int)*size);//выделяем память под смещения
		scounts=(int*)malloc(sizeof(int)*size);//выделяем память под количества элементов
	
		for(int g=0;g<size;++g){//рассчитываем число элементов и смещение
			displs[g]=borders[g].left*L;//рассчитываем смещения
			scounts[g]=borders[g].length*L;//рассчитываем количества элементов
		}
	}
	
	for(int i=0;i<(*borders).length*L;++i) c[i]=0.;//зануляем с
	
	//Рассылаем куски a на процессы
	MPI_Scatterv(a,scounts,displs,MPI_DOUBLE,a,(*borders).length*L,MPI_DOUBLE,0,MPI_COMM_WORLD);
	
	//Перемножаем матрицы
	Mprod(a,b,c,(*borders).length,L,L);
	
	//Считаем сумму квадратов
	for(int i=0;i<(*borders).length*L;++i)	*sum+=c[i]*c[i];
	//Собираем сумму на root
	MPI_Reduce(sum,resultnorm,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
	
	wt += MPI_Wtime();//замер времени
	return wt;//возвращаем время
}
