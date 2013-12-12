#include "core.h"


double* getData(border slice){
	double* temp=NULL;
	
	temp=malloc(sizeof(double)*slice.length);
	
	struct timeval t1;
	gettimeofday(&t1, NULL);
	srand(t1.tv_usec * t1.tv_sec + clock());
	
	for(long long int i=0;i<slice.length;++i)
		temp[i]=fRand();
		
	return temp;
}
