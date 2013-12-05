#include "core.h"

void cinematicPrintPoint(FILE *file,int timestep,double T,point *startPoint, long long int size, int fraps){
	point *temp;
	temp=startPoint;
	for(long long int i=0;i<size;temp++,++i)
		cinematicTimeStamp(file,timestep,(*temp).X,(*temp).Y,(*temp).U,T,fraps);
}

double core(double T,double L,int I,int NFRAMES){

	FILE *file;//Выходной файл
	double t;//Время
	
	point *CurPoints,*PrePoints;//точка сетки
	double dx=L/(double)(I-1),dy=L/(double)(I-1); //шаги
	double curx=0.,cury=0.;
	int ProcNum=0,rank=0;
	unsigned long int sizeC=(long unsigned int)(L/dx),sizeL=0;
	double dt = 0.9*dx*dx/N_K;
	int dfr = 0;
	long long int Tstep=0;
	border *borders; //границы разбиений
	MPI_Status	*status;
	
	borders=NULL;
	file=NULL;
	status=NULL;
	
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
	
	t=MPI_Wtime();
	//Рассылаем границы разбиений(они идут друг за другом номера элементов соответствуют номерам процессов)
	MPI_Scatter(borders,1,message_type,borders,1,message_type,0,MPI_COMM_WORLD);
	
	sizeL=(*borders).length;//число строк
	cury=(double)(*borders).left*dy;//номер первой строки
	
	fprintf(stderr,"%i %lf\n",rank,cury);
	if(rank!=0 && rank!=ProcNum-1){//на некрайних процессах на один буфер больше
		CurPoints=calloc((sizeL+2)*sizeC,sizeof(point));
		PrePoints=calloc((sizeL+2)*sizeC,sizeof(point));
	}
	else{
		CurPoints=calloc((sizeL+1)*sizeC,sizeof(point));
		PrePoints=calloc((sizeL+1)*sizeC,sizeof(point));
	}
	
	Build_derived_type_data(PrePoints,&data_type);
	
	
	dfr = (int)(T/dt)/NFRAMES;
	
	openFile(&file,"results.csv",rank);
	MPI_Barrier(MPI_COMM_WORLD);
	

	//строим сетку
	//TODO:Вынести в отдельную функцию
	if(rank==0)
	for(unsigned long int i=0;i<sizeL;++i){
		for(unsigned long int j=0;j<sizeC;++j){
			CurPoints[reductor(i,j,sizeC)].X+=curx;
			PrePoints[reductor(i,j,sizeC)].X+=curx;
			CurPoints[reductor(i,j,sizeC)].Y=cury;
			PrePoints[reductor(i,j,sizeC)].Y=cury;
			curx+=dx;
		}
		curx=0.;
		cury+=dy;
	}
	else
	for(unsigned long int i=1;i<sizeL+1;++i){
		for(unsigned long int j=0;j<sizeC;++j){
			CurPoints[reductor(i,j,sizeC)].X+=curx;
			PrePoints[reductor(i,j,sizeC)].X+=curx;
			CurPoints[reductor(i,j,sizeC)].Y=cury;
			PrePoints[reductor(i,j,sizeC)].Y=cury;
			curx+=dx;
		}
		curx=0.;
		cury+=dy;
	}

	
	//Начальные условия
	if(rank==0)
		for(unsigned long int i=0;i<(sizeL)*sizeC;++i)
			PrePoints[i].U=CurPoints[i].U=u0(PrePoints[i].X/L,PrePoints[i].Y/L);
	else
		for(unsigned long int i=sizeC;i<(sizeL+1)*sizeC;++i)
				PrePoints[i].U=CurPoints[i].U=u0(PrePoints[i].X/L,PrePoints[i].Y/L);
	
	for(double tau=0.;tau<T;tau+=dt){
		
		
		//Обмен сообщениями
		if(rank < ProcNum-1 && rank!=0)
			MPI_Send(&PrePoints[reductor(sizeL,0,sizeC)],sizeC,data_type,rank+1,0,MPI_COMM_WORLD);
		else
			if(rank==0)
				MPI_Send(&PrePoints[reductor(sizeL-1,0,sizeC)],sizeC,data_type,rank+1,0,MPI_COMM_WORLD);
		if(rank>0)
			MPI_Recv(&PrePoints[reductor(0,0,sizeC)],sizeC,data_type,rank-1,MPI_ANY_TAG,MPI_COMM_WORLD,status);
			
		if(rank > 0)
			MPI_Send(&PrePoints[reductor(1,0,sizeC)],sizeC,data_type,rank-1,0,MPI_COMM_WORLD);
		if(rank < ProcNum-1 && rank!=0)
			MPI_Recv(&PrePoints[reductor(sizeL+1,0,sizeC)],sizeC,data_type,rank+1,MPI_ANY_TAG,MPI_COMM_WORLD,status);
		else
			if(rank==0)
				MPI_Recv(&PrePoints[reductor(sizeL,0,sizeC)],sizeC,data_type,rank+1,MPI_ANY_TAG,MPI_COMM_WORLD,status);
		
		//Задание граничных условий
		if(rank==0){
			for(unsigned long int i=0;i<sizeC;++i)
				PrePoints[reductor(0,i,sizeC)].U=mu3(PrePoints[reductor(0,i,sizeC)].X/L,tau/T);
			for(unsigned long int i=0;i<sizeL;++i){
				PrePoints[reductor(i,0,sizeC)].U=mu1(PrePoints[reductor(i,0,sizeC)].Y/L,tau/T);
				PrePoints[reductor(i,sizeC-1,sizeC)].U=mu2(PrePoints[reductor(i,sizeC-1,sizeC)].Y/L,tau/T);
			}
		}
		else{
			if(rank==ProcNum-1)
				for(unsigned long int i=0;i<sizeC;++i)
					PrePoints[reductor(sizeL,i,sizeC)].U=mu4(PrePoints[reductor(sizeL,i,sizeC)].X/L,tau/T);	
			
			for(unsigned long int i=1;i<sizeL+1;++i){
				PrePoints[reductor(i,0,sizeC)].U=mu1(PrePoints[reductor(i,0,sizeC)].Y/L,tau/T);
				PrePoints[reductor(i,sizeC-1,sizeC)].U=mu2(PrePoints[reductor(i,sizeC-1,sizeC)].Y/L,tau/T);
			}
		}
		
		//Вывод в файл
		if(rank==0)
			cinematicPrintPoint(file,Tstep,tau,&PrePoints[reductor(0,0,sizeC)],sizeL*sizeC,dfr);
		else
			cinematicPrintPoint(file,Tstep,tau,&PrePoints[reductor(1,0,sizeC)],sizeL*sizeC,dfr);
		
		//Разностная схема
		if(rank == 0 || rank == ProcNum-1)
			M_U(CurPoints,PrePoints,sizeC-1,sizeL,sizeC,dt,T,dx,dy);
		else
			M_U(CurPoints,PrePoints,sizeC-1,sizeL+1,sizeC,dt,T,dx,dy);
		
		//Сохранение результа
		if(rank==0)
			for(unsigned long int i=0;i<sizeL;++i){
				for(unsigned long int j=0;j<sizeC;++j){
				PrePoints[reductor(i,j,sizeC)].U=CurPoints[reductor(i,j,sizeC)].U;
			}
		}
		else
			for(unsigned long int i=1;i<sizeL+1;++i){
				for(unsigned long int j=0;j<sizeC;++j){
				PrePoints[reductor(i,j,sizeC)].U=CurPoints[reductor(i,j,sizeC)].U;
			}
		}
	
		
		Tstep++;
	}
	
	t=MPI_Wtime()-t;

	closeFile(&file);
	free(CurPoints);
	free(PrePoints);
	return t;
} 
