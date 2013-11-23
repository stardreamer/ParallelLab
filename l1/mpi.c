#define _GNU_SOURCE
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>

void work(double * s,long long int begin,long long int end, int mode)
{
	switch(mode){
		case 0: //ch
			for(long long i = begin; i<=end;i+=2){
				*s += 1./powl(3.*(double)i-1,2);
			}
		break;
		case 1: //nech
			for(long long i = begin; i<=end;i+=2){
				*s -= 1./powl(3.*(double)i-1,2);
			}
		break;
	}
	
}
int main(int argc, char** argv)
{
	long long int N = 0;
	double sum=0.;
	int rank;
	int h,m;
	double buf;
	int size;
	MPI_Status status;
	if(argc >1) N = atoll(argv[1]);


	
	
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	if(size>N){
	if(rank == 0)
	fprintf(stderr,"N is bigger than number of proc \n");
	MPI_Finalize();
	return 0;
	}
	MPI_Barrier(MPI_COMM_WORLD);
	double wt=-MPI_Wtime();
	h=N/(size);
	m=(N%(size));
	
	if(m==0){
		work(&sum,(rank)*h+1,(rank+1)*h,1);
		work(&sum,(rank)*h+2,(rank+1)*h,0);
		fprintf(stderr,"proc %i sum %lf riad from %i to %i\n", rank,sum,(rank)*h+1,(rank+1)*h);
	}
	else{
		if(rank<m){
			work(&sum,rank*h+1,(rank+1)*h+1,1);
			work(&sum,rank*h+2,(rank+1)*h+1,0);
			fprintf(stderr,"proc %i sum %lf riad from %i to %i\n", rank,sum,rank*h+1,(rank+1)*h+1);
		}
		else{
			work(&sum,rank*h+2,(rank+1)*h+1,1);
			work(&sum,rank*h+3,(rank+1)*h+1,0);		
			fprintf(stderr,"proc %i sum %lf riad from %i to %i\n", rank,sum,rank*h+2,(rank+1)*h+1);	
		}
		
	}
	
	if(rank == 0){
		
		int curnum = size;
		while(curnum!=1){
				
		MPI_Recv(&buf,1,MPI_DOUBLE,MPI_ANY_SOURCE,9,MPI_COMM_WORLD,&status);
		curnum--;
		sum+=buf;
		}
		
	}
	
	else{	
	MPI_Send(&sum,1,MPI_DOUBLE,0,9,MPI_COMM_WORLD);
	}
	
	wt += MPI_Wtime();
	MPI_Barrier(MPI_COMM_WORLD);
	if(rank == 0)
		fprintf(stderr,"Answer %.10lf Time %lf(s)\n",sum,wt);
	
	MPI_Finalize();
	return 0;
}
