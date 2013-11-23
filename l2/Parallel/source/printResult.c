#include "support.h"

void printResult(int rank,border *borders,double *times,double sum,double resultnorm,const char* mes){
	fprintf(stderr,"------------------------------\nProc %i\n From: %i\n To:%i \n Length: %i\n Sum: %lf\n Time: %lf (s)\n Type: %s\n",rank,(*borders).left,(*borders).right,(*borders).length,sum,*times,mes);
	if(rank==0){
		resultnorm=sqrt(resultnorm);
		fprintf(stderr,"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\nType: %s\nNorm: %lf\nTime:%lf\n",mes,resultnorm,(*times));
	}
	
}
