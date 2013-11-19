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
		break;
		default:
		break;
	}
}
