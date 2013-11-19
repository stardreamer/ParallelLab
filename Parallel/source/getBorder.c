#include "support.h"

void getBorder(border* borders,int N,int size,int mode,double *times){
	int h,m;
	double R=0.;
	h=N/(size);
	m=(N%(size));
	switch(mode){
		case SIMPLE_BREAK:
			if(m==0){
				for(int g=0;g<size;g++){
					borders[g].left=(g)*h;
					borders[g].right=(g+1)*h-1;
					borders[g].length=borders[g].right-borders[g].left+1;
				}
			}
			else{

				for(int g=0;g<m;++g){
					borders[g].left=g*(h+1);
					borders[g].right=(g+1)*h+g;
					borders[g].length=borders[g].right-borders[g].left+1;
				}

				for(int g=m;g<size;++g){
					borders[g].left=(g)*h+m;
					borders[g].right=(g)*h+h+m-1;
					borders[g].length=borders[g].right-borders[g].left+1;
				}		
			}
		break;
		case ADDAPRIVE_BREAK:
			for(int i=0;i<size;++i)
				R+=1./times[i];
			double prevrightborder=0.;//предыдущая правая граница
			for(int i=0;i<m;++i){
				borders[i].length=N*(1./(times[i]*R))+1;
				borders[i].left=prevrightborder;
				borders[i].right=prevrightborder+(borders[i].length-1);
				prevrightborder=borders[i].right+1;
			}
			for(int i=m;i<size;++i){
				borders[i].length=N*(1./(times[i]*R));
				borders[i].left=prevrightborder;
				borders[i].right=prevrightborder+(borders[i].length-1);
				prevrightborder=borders[i].right+1;
			}
		break;
		default:
		break;
	}
}
