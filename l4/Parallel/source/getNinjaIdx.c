#include "datatypes.h"

/*
 * 
 * name: getNinjaIdx
 * @param wholeArray указатель на массив
 * @param lider ведущий элемент
 * @return индекс первого элемента большего чем ведущий
 * 
 */
long long int getNinjaIdx(array* wholeArray, double lider){
	double *temp=(*wholeArray).Arr;
	long long int i=0;
	
	if(temp!=NULL)
		for(;(*temp<lider)&&(i<((*wholeArray).length));temp++, i++);
	
	
	return i;
}
