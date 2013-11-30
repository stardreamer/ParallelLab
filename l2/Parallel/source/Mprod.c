#include "support.h"

void Mprod(double *a,double *b,double *c,int l1,int l2,int l3){
	double abuf=0.,*a1;
	a1=a;
	//Перемножаем матрицы
	
	for(int i=0;i<l1;++i){
		for(int k=0;k<l2;++k){
			abuf=*(a1++);//по a мы движемся последовательно, значит не нужно использовать макрос
			for(int j=0;j<l3;++j)
				c[reductor(i,j,l3)]+=abuf*b[reductor(k,j,l3)];		
		}
	}
}
