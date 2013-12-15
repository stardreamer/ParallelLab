#include "datatypes.h"

/*
 * 
 * name: MyBubbleSort
 * @param unsortedArray указатель на неотсортированный массив
 * @return void
 * 
 */

int MyBubbleSort(array* unsortedArray){
	int counter=0;
	int size=unsortedArray->length;
	double *arr=unsortedArray->Arr;
	double buf=0.;
	
	for(int i=0;i<size;++i)
		for(int j=size-1;j>i;--j){
			if(arr[j]<arr[j-1]){
				counter++;
				buf=arr[j];
				arr[j]=arr[j-1];
				arr[j-1]=buf;
			}
		}
	return counter;
}
