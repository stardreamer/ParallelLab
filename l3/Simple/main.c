#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#define M_PI 3.14159265358979323846
#define N_K 0.00001864471911
#define NFRAMES 100
struct Point{
	double X,Y,U,dx,dy,dt;
	int Tstep;
};
typedef struct Point point;

inline double lambda(double u) __attribute__((always_inline));
inline double rho(double u) __attribute__((always_inline));
inline double c(double u) __attribute__((always_inline));
inline double u0(double x,double y) __attribute__((always_inline));
inline double mu1(double y, double tau) __attribute__((always_inline));
inline double mu2(double y, double tau) __attribute__((always_inline));
inline double mu3(double x, double tau) __attribute__((always_inline));
inline double mu4(double x, double tau) __attribute__((always_inline));
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

inline double mu1(double y, double tau){
	return 300.+700.*tau/(y+1.);
}

inline double mu2(double y, double tau){
	return 300.*exp(-0.125*y*tau);
}

inline double mu3(double x, double tau){
	return 340.*x+(1.-x)*(300.+700.*tau);
}

inline double mu4(double x, double tau){
	return 340.*x*exp(-0.125*tau)+(1.-x)*(300.+350.*tau);
}

void M_U(point *curPoint,point *lastPoint,int lx,int ly,int L,double dt){
	for(int i=1;i<lx;++i)
		for(int j=1;j<ly;++j)
			curPoint[reductor(i,j,L)].U=
				lastPoint[reductor(i,j,L)].U+
				(dt/(c(lastPoint[reductor(i,j,L)].U)*rho(lastPoint[reductor(i,j,L)].U)))*
				(
				lambda((lastPoint[reductor(i+1,j,L)].U+lastPoint[reductor(i,j,L)].U)/2.)*(lastPoint[reductor(i+1,j,L)].U-lastPoint[reductor(i,j,L)].U)/lastPoint[reductor(i,j,L)].dx*lastPoint[reductor(i,j,L)].dx-
				lambda((lastPoint[reductor(i-1,j,L)].U+lastPoint[reductor(i,j,L)].U)/2.)*(lastPoint[reductor(i,j,L)].U-lastPoint[reductor(i-1,j,L)].U)/lastPoint[reductor(i,j,L)].dx*lastPoint[reductor(i,j,L)].dx+
				lambda((lastPoint[reductor(i,j+1,L)].U+lastPoint[reductor(i,j,L)].U)/2.)*(lastPoint[reductor(i,j+1,L)].U-lastPoint[reductor(i,j,L)].U)/lastPoint[reductor(i,j,L)].dy*lastPoint[reductor(i,j,L)].dy-
				lambda((lastPoint[reductor(i,j-1,L)].U+lastPoint[reductor(i,j,L)].U)/2.)*(lastPoint[reductor(i,j,L)].U-lastPoint[reductor(i,j-1,L)].U)/lastPoint[reductor(i,j,L)].dy*lastPoint[reductor(i,j,L)].dy
				);
		
}


int main(int argc, char *argv[]){
	FILE *file; 
	clock_t t;//Время
	int counter=0;
	double L=1e-2;
	int I1=100,I2=500,I3=1000;
	double dx=L/(double)I1,dy=L/(double)I1;
	double curx=0.,cury=0.;
	unsigned long size=(long unsigned int)(L/dx);//(long unsigned int)((L*L)/(dx*dy))
	double dt = 0.9*dx*dx/N_K;
	point *CurPoints,*PrePoints;
	double T=argc>1?atof(argv[1]):1.;//Получаем размер матрицы. По умолчанию 10.
	CurPoints=malloc(sizeof(point)*size*size);
	PrePoints=malloc(sizeof(point)*size*size);
	file = fopen("results.csv","w");
	int dfr = (int)(T/dt)/NFRAMES; 
	
	for(int i=0;i<size;++i){
		for(int j=0;j<size;++j){
			PrePoints[reductor(i,j,size)].X=CurPoints[reductor(i,j,size)].X+=curx;
			PrePoints[reductor(i,j,size)].Y=CurPoints[reductor(i,j,size)].Y=cury;
			PrePoints[reductor(i,j,size)].Tstep=CurPoints[reductor(i,j,size)].Tstep=0;
			curx+=dx;
		}
		curx=0.;
		cury+=dy;

	}
	
	for(int i=0;i<size*size;++i){
		PrePoints[i].U=u0(PrePoints[i].X/L,PrePoints[i].Y/L);
	
	PrePoints[i].dx=PrePoints[i].dy=CurPoints[i].dt=0.1;}
	t=clock();
	fprintf(file,"\"TStep\";\"x\";\"y\";\"U\";\"dt\"\n");
	for(double tau=0.;tau<T;tau+=dt){
		for(int i=0;i<size;++i){
			PrePoints[reductor(0,i,size)].U=mu3(PrePoints[reductor(0,i,size)].X/L,tau/T);
			PrePoints[reductor(size-1,i,size)].U=mu4(PrePoints[reductor(size-1,i,size)].X/L,tau/T);
			PrePoints[reductor(i,0,size)].U=mu1(PrePoints[reductor(i,0,size)].Y/L,tau/T);
			PrePoints[reductor(i,size-1,size)].U=mu2(PrePoints[reductor(i,size-1,size)].Y/L,tau/T);
		}
		
		M_U(CurPoints,PrePoints,size-1,size-1,size,dt);
		
		for(int i=0;i<size*size;++i){
			if(dfr == 0){
				fprintf(file,"%i;%lf;%lf;%lf;%lf\n",PrePoints[i].Tstep,PrePoints[i].X,PrePoints[i].Y,PrePoints[i].U,PrePoints[i].dt);
				++counter;
			}
			else{
				if(PrePoints[i].Tstep % dfr == 0){
					++counter;
					fprintf(file,"%i;%lf;%lf;%lf;%lf\n",PrePoints[i].Tstep,PrePoints[i].X,PrePoints[i].Y,PrePoints[i].U,PrePoints[i].dt);
				}
			}
				
			PrePoints[i].U=CurPoints[i].U;
			PrePoints[i].dt=tau;
			++PrePoints[i].Tstep;
		}

	}
	
	
	t=clock()-t;
	/*for(int i=0;i<size;i++){
		printf("% lf %lf %lf\n",CurPoints[i].X,CurPoints[i].Y,CurPoints[i].U);
	}*/
	fclose(file);
	fprintf(stderr,"\nTime %lf, %i, %i,%lf, %lf\n",((float)t)/CLOCKS_PER_SEC,counter,dt, dfr,T);
	free(CurPoints);
	free(PrePoints);
	return 0;
}
