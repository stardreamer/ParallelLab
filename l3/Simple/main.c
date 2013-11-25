#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#define M_PI 3.14159265358979323846
struct Point{
	double X,Y,U,dx,dy,dt;
	int Tstep;
};
typedef struct Point point;

inline double lambda(double u) __attribute__((always_inline));
inline double rho(double u) __attribute__((always_inline));
inline double c(double u) __attribute__((always_inline));
inline double u0(double x,double y) __attribute__((always_inline));
inline int reductor(int i,int j,int l) __attribute__((always_inline));

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
	for(int i=1;i<lx;++i)
		for(int j=1;j<ly;++j)
			curPoint[reductor(i,j,L)].U=
				lastPoint[reductor(i,j,L)].U+
				(curPoint[reductor(i,j,L)].dt/(c(lastPoint[reductor(i,j,L)].U)*rho(lastPoint[reductor(i,j,L)].U)))*
				(
				lambda((lastPoint[reductor(i+1,j,L)].U+lastPoint[reductor(i,j,L)].U)/2.)*(lastPoint[reductor(i+1,j,L)].U-lastPoint[reductor(i,j,L)].U)/lastPoint[reductor(i,j,L)].dx*lastPoint[reductor(i,j,L)].dx-
				lambda((lastPoint[reductor(i-1,j,L)].U+lastPoint[reductor(i,j,L)].U)/2.)*(lastPoint[reductor(i,j,L)].U-lastPoint[reductor(i-1,j,L)].U)/lastPoint[reductor(i,j,L)].dx*lastPoint[reductor(i,j,L)].dx+
				lambda((lastPoint[reductor(i,j+1,L)].U+lastPoint[reductor(i,j,L)].U)/2.)*(lastPoint[reductor(i,j+1,L)].U-lastPoint[reductor(i,j,L)].U)/lastPoint[reductor(i,j,L)].dy*lastPoint[reductor(i,j,L)].dy-
				lambda((lastPoint[reductor(i,j-1,L)].U+lastPoint[reductor(i,j,L)].U)/2.)*(lastPoint[reductor(i,j,L)].U-lastPoint[reductor(i,j-1,L)].U)/lastPoint[reductor(i,j,L)].dy*lastPoint[reductor(i,j,L)].dy
				);
		
}


int main(int argc, char *argv[]){
	clock_t t;//Время
	double L=1e-2;
	int I1=100,I2=500,I3=1000;
	double dx=L/(double)I1,dy=L/(double)I1;
	unsigned long size=(long unsigned int)(L/dx);//(long unsigned int)((L*L)/(dx*dy))
	point *CurPoints,*PrePoints;
	double T=argc>1?atoi(argv[1]):1.;//Получаем размер матрицы. По умолчанию 10.
	CurPoints=malloc(sizeof(point)*size*size);
	PrePoints=malloc(sizeof(point)*size*size);
	
	for(int i=0;i<size;++i){
		for(int j=0;j<size;++j){
			PrePoints[reductor(i,j,size)].X=CurPoints[reductor(i,j,size)].X=(double)i*dx;
			PrePoints[reductor(i,j,size)].Y=CurPoints[reductor(i,j,size)].Y=(double)i*dy;
		}
	}
	
	for(int i=0;i<size*size;++i){
		PrePoints[i].U=u0(PrePoints[i].X,PrePoints[i].Y);
	
	PrePoints[i].dx=PrePoints[i].dy=CurPoints[i].dt=0.1;}
	t=clock();
	for(double tau=0.;tau<;tau=+0.1){
		for(int i=0;i<size;++i){
			PrePoints[reductor(0,i,size)].U=
		}
		M_U(CurPoints,PrePoints,size-1,size-1,size-1);
	}
	
	
	t=clock()-t;
	for(int i=0;i<size;i++){
		printf("% lf %lf %lf\n",CurPoints[i].X,CurPoints[i].Y,CurPoints[i].U);
	}
	fprintf(stderr,"\ndx %lf dy %lf time %lf\n",dx,dy,((float)t)/CLOCKS_PER_SEC);
	free(CurPoints);
	free(PrePoints);
	return 0;
}
