#include "core.h"

/*
 * 
 * name: MyBubbleSort
 * @param unsortedArray указатель на неотсортированный массив
 * @return void
 * 
 */

void MyBubbleSort(array* unsortedArray){
	long long int size = (*unsortedArray).myBorder.length;
	double *arr=(*unsortedArray).Arr;
	double buf=0.;
	
	for(long long int i=0;i<size;++i)
		for(long long int j=size-1;j>i;--j){
			if(arr[j]<arr[j-1]){
				buf=arr[j];
				arr[j]=arr[j-1];
				arr[j-1]=buf;
			}
		}
	
}