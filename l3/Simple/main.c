#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#define M_PI 3.14159265358979323846
struct Point{
	double X,Y,U,dt;
	int Tstep;
};
typedef struct Point point;

inline double lambda(double u) __attribute__((always_inline));
inline double rho(double u) __attribute__((always_inline));
inline double c(double u) __attribute__((always_inline));
inline double u0(double x,double y) __attribute__((always_inline));
inline int reductor(int i,int j) __attribute__((always_inline));

inline int reductor(int i,int j,int l){
	return (l*i+j);
}

inline double lambda(double u){
	return 1.45+2.3*1e-2*u-2*1e-6*u*u;
}

inline double rho(double u){
	return 7860.+41500./u;
}

inline double c(double u){
	return 1./(2.25*1e-3-6.08*1e-10*u*u);
}

inline double u0(double x,double y){
	return (300.+400.*x)*(sin(M_PI*y)+1.);
}


void M_U(point *curPoint,point *lastPoint,int lx,int ly,int L){
	for(int i=0;i<lx;++i)
		for(int j=0;j<ly;++j)
			curPoint[reductor(i,j,L)].U=
				lastPoint(reductor(i,j,L))+
				(curPoint[reductor(i,j,L)].dt/c(lastPoint(reductor(i,j,L)))*rho(lastPoint(reductor(i,j,L))))*
				()
		
}


int main(int argc, char *argv[]){
	clock_t t;//Время
	double L=1e-2;
	int I1=100,I2=500,I3=1000;
	double dx=L/(double)I1,dy=L/(double)I1;
	unsigned long size=1000;//(long unsigned int)((L*L)/(dx*dy))
	point *CurPoints,*PrePoints;
	double T=argc>1?atoi(argv[1]):1.;//Получаем размер матрицы. По умолчанию 10.
	CurPoints=malloc(sizeof(point)*size);
	PrePoints=malloc(sizeof(point)*size);
	
	for(int i=0;i<size;i++){
		PrePoints[i].X=CurPoints[i].X=(double)i*0.1;
		PrePoints[i].Y=CurPoints[i].Y=(double)i*0.1;
	}
	
	
	t=clock();
	M_U(CurPoints,PrePoints,size);
	
	
	
	t=clock()-t;
	for(int i=0;i<size;i++){
		printf("% lf %lf %lf\n",CurPoints[i].X,CurPoints[i].Y,CurPoints[i].U);
	}
	fprintf(stderr,"\ndx %lf dy %lf time %lf\n",dx,dy,((float)t)/CLOCKS_PER_SEC);
	
	return 0;
}
