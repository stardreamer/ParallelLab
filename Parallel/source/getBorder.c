#include "support.h"

void getBorder(border* borders,int N,int size,int mode){
	int h,m;
	switch(mode){
		case SIMPLE_BREAK:
			h=N/(size);
			m=(N%(size));

			if(m==0){
				for(int g=0;g<size;g++){
					borders[g].left=(g)*h;
					borders[g].right=(g+1)*h-1;
					borders[g].length=h;
				}
			}
			else{

				for(int g=0;g<m;++g){
					borders[g].left=g*h;
					borders[g].right=(g+1)*h;
					borders[g].length=(h+1);
				}

				for(int g=m;g<size;++g){
					borders[g].left=g*h+1;
					borders[g].right=(g+1)*h;
					borders[g].length=h;
				}		
			}
		break;
		case ADDAPRIVE_BREAK:
		break;
		default:
		break;
	}
}
