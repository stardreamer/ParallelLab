#include "core.h"

border getNum(int rank,long long int N,int size){
	long long  int h,m;
	border temp;
	h=N/(size);
	m=(N%(size));
	
	if(m==0){
			temp.left=(rank)*h;
			temp.right=(rank+1)*h-1;
			temp.length=temp.right-temp.left+1;
	}
	else{
		if(rank<m){
			temp.left=rank*(h+1);
			temp.right=(rank+1)*h+rank;
			temp.length=temp.right-temp.left+1;
		}
		else{
			temp.left=(rank)*h+m;
			temp.right=(rank)*h+h+m-1;
			temp.length=temp.right-temp.left+1;
		}	
	}
	
	return temp;
}
