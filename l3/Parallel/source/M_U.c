#include "core.h"


void M_U(point *curPoint,point *lastPoint,int lx,int ly,int L,double dt,double T,double Len,double dx,double dy){
	double l1=1/Len;
	for(int i=1;i<ly;i++)
		for(int j=1;j<lx;j++)
			curPoint[reductor(i,j,L)].U=
				lastPoint[reductor(i,j,L)].U+
				(T*l1*l1*dt/(c(lastPoint[reductor(i,j,L)].U)*rho(lastPoint[reductor(i,j,L)].U)))*
				(
				lambda((lastPoint[reductor(i+1,j,L)].U+lastPoint[reductor(i,j,L)].U)/2.)*(lastPoint[reductor(i+1,j,L)].U-lastPoint[reductor(i,j,L)].U)/dx*dx-
				lambda((lastPoint[reductor(i-1,j,L)].U+lastPoint[reductor(i,j,L)].U)/2.)*(lastPoint[reductor(i,j,L)].U-lastPoint[reductor(i-1,j,L)].U)/dx*dx+
				lambda((lastPoint[reductor(i,j+1,L)].U+lastPoint[reductor(i,j,L)].U)/2.)*(lastPoint[reductor(i,j+1,L)].U-lastPoint[reductor(i,j,L)].U)/dy*dy-
				lambda((lastPoint[reductor(i,j-1,L)].U+lastPoint[reductor(i,j,L)].U)/2.)*(lastPoint[reductor(i,j,L)].U-lastPoint[reductor(i,j-1,L)].U)/dy*dy
				);

}

