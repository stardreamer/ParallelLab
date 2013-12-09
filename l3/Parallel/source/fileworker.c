#include "fileworker.h"

void deleteFile(const char* filename){
		unlink(filename);
}
void openFile(FILE **file,const char* filename, int rank,int param){

	*file = fopen(filename,"a");
		setvbuf(*file, NULL, _IONBF, 0);
	if(rank == 0 && param == PYTHONPLOT_F)
		fprintf(*file,"\"TStep\";\"x\";\"y\";\"U\";\"T\"\n");
}

void printTimeStamp(FILE *file,int timestep,double x,double y,double u,double time){
	fprintf(file,"%i;%lf;%lf;%lf;%lf\n",timestep,x,y,u,time);
}

void cinematicTimeStamp(FILE *file,int timestep,double x,double y,double u,double time,int fraps){
	if(fraps == 0 || timestep % fraps == 0)
		fprintf(file,"%i;%lf;%lf;%lf;%lf\n",timestep,x,y,u,time);
}

void closeFile(FILE **file){
	fclose(*file);
}


