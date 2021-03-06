#include "core.h"

void cinematicPrintPoint(FILE *file,int timestep,double T,point *startPoint, long long int size, int fraps){
	point *temp;
	temp=startPoint;
	for(long long int i=0;i<size;temp++,++i)
		cinematicTimeStamp(file,timestep,(*temp).X,(*temp).Y,(*temp).U,T,fraps);
}

void gnuPrintPoint(FILE *file,point *startPoint, long long int size){
	point *temp;
	temp=startPoint;
	for(long long int i=0;i<size;temp++,++i)
		fprintf(file,"%lf %lf %lf\n",(*temp).Y,(*temp).X,(*temp).U);
}

double core(double T,double L,int I,int NFRAMES){

	FILE *file;//Выходной файл
	FILE *gnuplot;
	double t=0.;//Время
	
	point *CurPoints,*PrePoints,*bufPoints;//точка сетки
	double dx=L/(double)(I-1),dy=L/(double)(I-1); //шаги
	double curx=0.,cury=0.;
	int ProcNum=0,rank=0;
	unsigned long int sizeC=(long unsigned int)(L/dx),sizeL=0;
	double dt = 0.9*dx*dx/N_K;
	int dfr = 0;
	long long int Tstep=0;
	border *borders; //границы разбиений
	border *tempborder;
	MPI_Status	*status;
	
	borders=NULL;
	file=NULL;
	status=NULL;
	
	MPI_Datatype message_type/*Тут будет храниться представление структуры border*/;
	MPI_Datatype data_type/*Тут будет храниться представление структуры point*/;
	
	MPI_Comm_size(MPI_COMM_WORLD,&ProcNum);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	if(ProcNum-1>sizeC) ProcNum=sizeC+1;
			
	if(rank<ProcNum){

	if(rank==0){
		borders=(border*)malloc(sizeof(border)*ProcNum);
		getBorder(borders,sizeC,ProcNum);
		tempborder=(border*)malloc(sizeof(border));
	}
	else
		tempborder=(border*)malloc(sizeof(border));
	

	Build_derived_type(	tempborder,&message_type);//Инициализируем свой mpi тип

	t=MPI_Wtime();
	//Рассылаем границы разбиений(они идут друг за другом номера элементов соответствуют номерам процессов)
	MPI_Scatter(borders,1,message_type,tempborder,1,message_type,0,MPI_COMM_WORLD);

	if(rank!=0)	
		borders=tempborder;
	sizeL=(*borders).length;//число строк
	cury=(double)(*borders).left*dy;//номер первой строки


	if(rank!=0 && rank!=ProcNum-1){//на некрайних процессах на один буфер больше
		CurPoints=calloc((sizeL+2)*sizeC,sizeof(point));
		PrePoints=calloc((sizeL+2)*sizeC,sizeof(point));
	}
	else{
		CurPoints=calloc((sizeL+1)*sizeC,sizeof(point));
		PrePoints=calloc((sizeL+1)*sizeC,sizeof(point));
	}
	
	Build_derived_type_data(PrePoints,&data_type);
	
	if(NFRAMES!=0)
	dfr = (int)(T/dt)/NFRAMES;
	
	if(rank==0){
		deleteFile("results.csv");
		deleteFile("gnuresults.csv");
	}
	
	MPI_Barrier(MPI_COMM_WORLD);
	openFile(&file,"results.csv",rank,PYTHONPLOT_F);
	openFile(&gnuplot,"gnuresults.csv",rank,GNUPLOT_F);
	MPI_Barrier(MPI_COMM_WORLD);
	

	//строим сетку
	//TODO:Вынести в отдельную функцию
	if(rank==0)
	for(unsigned long int i=0;i<sizeL;i++){
		for(unsigned long int j=0;j<sizeC;j++){
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
	for(unsigned long int i=1;i<sizeL+1;i++){
		for(unsigned long int j=0;j<sizeC;j++){
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
		for(unsigned long int i=0;i<(sizeL)*sizeC;i++){
			CurPoints[i].U=u0(PrePoints[i].X/L,PrePoints[i].Y/L);
			PrePoints[i].U=CurPoints[i].U;
		}
	else
		for(unsigned long int i=sizeC;i<(sizeL+1)*sizeC;i++){
				CurPoints[i].U=u0(PrePoints[i].X/L,PrePoints[i].Y/L);
				PrePoints[i].U=CurPoints[i].U;
			}
		
	
	for(double tau=0.;tau<T;tau+=dt){
		
		//Задание граничных условий
		if(rank==0 ){
			for(unsigned long int i=0;i<sizeC;i++){
				PrePoints[reductor(0,i,sizeC)].U=mu3(PrePoints[reductor(0,i,sizeC)].X/L,tau/T);
				CurPoints[reductor(0,i,sizeC)].U=PrePoints[reductor(0,i,sizeC)].U;
				}
			for(unsigned long int i=0;i<sizeL;i++){
				PrePoints[reductor(i,0,sizeC)].U=mu1(PrePoints[reductor(i,0,sizeC)].Y/L,tau/T);
				PrePoints[reductor(i,sizeC-1,sizeC)].U=mu2(PrePoints[reductor(i,sizeC-1,sizeC)].Y/L,tau/T);
				CurPoints[reductor(i,0,sizeC)].U=PrePoints[reductor(i,0,sizeC)].U;
				CurPoints[reductor(i,sizeC-1,sizeC)].U=PrePoints[reductor(i,sizeC-1,sizeC)].U;
			}
			if(ProcNum == 1)
				for(unsigned long int i=0;i<sizeC;i++){
					PrePoints[reductor(sizeL,i,sizeC)].U=mu4(PrePoints[reductor(sizeL-1,i,sizeC)].X/L,tau/T);	
					CurPoints[reductor(sizeL,i,sizeC)].U=PrePoints[reductor(sizeL-1,i,sizeC)].U;
				}
		}
		else{
			if(rank==ProcNum-1)
				for(unsigned long int i=0;i<sizeC;i++){
					PrePoints[reductor(sizeL,i,sizeC)].U=mu4(PrePoints[reductor(sizeL,i,sizeC)].X/L,tau/T);	
					CurPoints[reductor(sizeL,i,sizeC)].U=PrePoints[reductor(sizeL,i,sizeC)].U;
				}
			
			for(unsigned long int i=1;i<sizeL+1;i++){
				PrePoints[reductor(i,0,sizeC)].U=mu1(PrePoints[reductor(i,0,sizeC)].Y/L,tau/T);
				PrePoints[reductor(i,sizeC-1,sizeC)].U=mu2(PrePoints[reductor(i,sizeC-1,sizeC)].Y/L,tau/T);
				CurPoints[reductor(i,0,sizeC)].U=PrePoints[reductor(i,0,sizeC)].U;
				CurPoints[reductor(i,sizeC-1,sizeC)].U=PrePoints[reductor(i,sizeC-1,sizeC)].U;
			}
		}
		
		
		//Обмен сообщениями
		if(rank < (ProcNum-1) ){
			if(rank!=0)
				MPI_Send(&PrePoints[reductor(sizeL,0,sizeC)],sizeC,data_type,rank+1,0,MPI_COMM_WORLD);
			else
				MPI_Send(&PrePoints[reductor(sizeL-1,0,sizeC)],sizeC,data_type,rank+1,0,MPI_COMM_WORLD);
		}
		if(rank>0)
			MPI_Recv(&PrePoints[reductor(0,0,sizeC)],sizeC,data_type,rank-1,MPI_ANY_TAG,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			
		if(rank > 0)
			MPI_Send(&PrePoints[reductor(1,0,sizeC)],sizeC,data_type,rank-1,0,MPI_COMM_WORLD);
		if(rank < ProcNum-1 ){
			if(rank!=0)
				MPI_Recv(&PrePoints[reductor(sizeL+1,0,sizeC)],sizeC,data_type,rank+1,MPI_ANY_TAG,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			else
				MPI_Recv(&PrePoints[reductor(sizeL,0,sizeC)],sizeC,data_type,rank+1,MPI_ANY_TAG,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			}
		
		if(NFRAMES!=0){
		//Вывод в файл
		if(rank==0)
			cinematicPrintPoint(file,Tstep,tau,&PrePoints[reductor(0,0,sizeC)],sizeL*sizeC,dfr);
		else
			cinematicPrintPoint(file,Tstep,tau,&PrePoints[reductor(1,0,sizeC)],sizeL*sizeC,dfr);
		}
		
		//Разностная схема
		if(rank == 0 || rank == ProcNum-1)
			M_U(CurPoints,PrePoints,sizeC-1,sizeL,sizeC,dt,T,L,dx,dy);
		else
			M_U(CurPoints,PrePoints,sizeC-1,sizeL+1,sizeC,dt,T,L,dx,dy);
		
		//Сохранение результа
		/*if(rank==0)
			for(unsigned long int i=0;i<sizeL;i++){
				for(unsigned long int j=0;j<sizeC;j++){
				PrePoints[reductor(i,j,sizeC)].U=CurPoints[reductor(i,j,sizeC)].U;
			}
		}
		else
			for(unsigned long int i=1;i<sizeL+1;i++){
				for(unsigned long int j=0;j<sizeC;j++){
				PrePoints[reductor(i,j,sizeC)].U=CurPoints[reductor(i,j,sizeC)].U;
			}
		}*/
		
		bufPoints=PrePoints;
		PrePoints=CurPoints;
		CurPoints=bufPoints;
		
		
		Tstep++;
	}
	
	
	t=MPI_Wtime()-t;
	
	for(int i=0;i< ProcNum;i++){
		if(rank==i){
			if(i==0)
				gnuPrintPoint(gnuplot,&PrePoints[reductor(0,0,sizeC)],sizeL*sizeC);
			else
				gnuPrintPoint(gnuplot,&PrePoints[reductor(1,0,sizeC)],sizeL*sizeC);
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}
	closeFile(&gnuplot);
	closeFile(&file);
	free(CurPoints);
	free(PrePoints);
}
	return t;
} 
