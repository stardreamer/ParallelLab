#include "core.h"


double core(double T,double L,int I,int NFRAMES){
	FILE *file;//Выходной файл
	double t;//Время
	
	point *CurPoints,*PrePoints;//точка сетки
	double dx=L/(double)I,dy=L/(double)I; //шаги
	double curx=0.,cury=0.;
	int ProcNum=0,rank=0;
	unsigned long int sizeC=(long unsigned int)(L/dx),sizeL=0;
	double dt = 0.9*dx*dx/N_K;
	int dfr = 0;
	int counter=0;
	long long int Tstep=0;
	point *endPoint; 
	border *borders; //границы разбиений
	MPI_Status	*status;
	
	borders=NULL;
	endPoint=NULL;
	file=NULL;
	
	MPI_Datatype message_type/*Тут будет храниться представление структуры border*/;
	MPI_Datatype data_type/*Тут будет храниться представление структуры point*/;
	
	MPI_Comm_size(MPI_COMM_WORLD,&ProcNum);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	
	
	
	
	if(rank==0){
		borders=(border*)malloc(sizeof(border)*ProcNum);
		getBorder(borders,sizeC,ProcNum);
	}
	else
		borders=(border*)malloc(sizeof(border));
	
	
	Build_derived_type(borders,&message_type);//Инициализируем свой mpi тип
	//Рассылаем границы разбиений(они идут друг за другом номера элементов соответствуют номерам процессов)
	MPI_Scatter(borders,1,message_type,borders,1,message_type,0,MPI_COMM_WORLD);
	sizeL=(*borders).length;
	cury=(double)(*borders).left*dy;
	
	fprintf(stderr,"%i %lf\n",rank,cury);
	CurPoints=calloc((sizeL+1)*sizeC,sizeof(point));
	PrePoints=calloc((sizeL+1)*sizeC,sizeof(point));
	
	Build_derived_type_data(PrePoints,&data_type);
	
	endPoint=&PrePoints[reductor(0,sizeL,sizeC)];
	
	dfr = (int)(T/dt)/NFRAMES;

		file = fopen("results.csv","a");
	if(rank == 0){
		fprintf(file,"\"TStep\";\"x\";\"y\";\"U\";\"T\"\n");
	}
	MPI_Barrier(MPI_COMM_WORLD);
	
	setvbuf(file, NULL, _IONBF, 0);
	//строим сетку
	for(unsigned long int i=0;i<sizeL;++i){
		for(unsigned long int j=0;j<sizeC;++j){
			PrePoints[reductor(i,j,sizeC)].X=CurPoints[reductor(i,j,sizeC)].X+=curx;
			PrePoints[reductor(i,j,sizeC)].Y=CurPoints[reductor(i,j,sizeC)].Y=cury;
			curx+=dx;
		}
		curx=0.;
		cury+=dy;
	}

	for(unsigned long int i=0;i<(sizeL+1)*sizeC;++i)
		PrePoints[i].U=u0(PrePoints[i].X/L,PrePoints[i].Y/L);


	
	t=MPI_Wtime();
	for(double tau=0.;tau<T;tau+=dt){
		//Задаём граничные условия
		if(rank==0)
			for(unsigned long int i=0;i<sizeC;++i)
				PrePoints[reductor(0,i,sizeC)].U=mu3(PrePoints[reductor(0,i,sizeC)].X/L,tau/T);
			
		
		if(rank==ProcNum-1)
			for(unsigned long int i=0;i<sizeC;++i)
				PrePoints[reductor(sizeL-1,i,sizeC)].U=mu4(PrePoints[reductor(sizeL-1,i,sizeC)].X/L,tau/T);	
		
		for(unsigned long int i=0;i<sizeL;++i){
			PrePoints[reductor(i,0,sizeC)].U=mu1(PrePoints[reductor(i,0,sizeC)].Y/L,tau/T);
			PrePoints[reductor(i,sizeC-1,sizeC)].U=mu2(PrePoints[reductor(i,sizeC-1,sizeC)].Y/L,tau/T);
		}

		//Разностная схема
		M_U(CurPoints,PrePoints,sizeC-1,sizeL-1,sizeC,dt,T,dx,dy);
		
		//Сохранение результа
		for(unsigned long int i=0;i<sizeL*sizeC;++i){
			
			if(dfr == 0){
				fprintf(file,"%i;%lf;%lf;%lf;%lf\n",Tstep,PrePoints[i].X,PrePoints[i].Y,PrePoints[i].U,PrePoints[i].T);
				++counter;
			}
			else{
				if(Tstep % dfr == 0){
					++counter;
					fprintf(file,"%i;%lf;%lf;%lf;%lf\n",Tstep,PrePoints[i].X,PrePoints[i].Y,PrePoints[i].U,PrePoints[i].T);
				}
			}
			
			PrePoints[i].U=CurPoints[i].U;
			PrePoints[i].T=tau;
			
		}
		++Tstep;
		if(rank < ProcNum-1)
			MPI_Send(endPoint,sizeC,data_type,rank+1,0,MPI_COMM_WORLD);
		if(rank > 0)
			MPI_Recv(PrePoints,sizeC,data_type,rank-1,MPI_ANY_TAG,MPI_COMM_WORLD,status);
	}


	t=MPI_Wtime()-t;

	fclose(file);
	free(CurPoints);
	free(PrePoints);
	return t;
} 
