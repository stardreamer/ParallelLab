#include "datatypes.h"

/*
 * 
 * name: getNinjaIdx
 * @param wholeArray указатель на массив
 * @param lider ведущий элемент
 * @return индекс первого элемента большего чем ведущий
 * 
 */
int getNinjaIdx(array* wholeArray, double lider){
	double *temp=wholeArray->Arr;
	int i=0;
	
	if(temp!=NULL)
		for(;(*temp<lider)&&(i<(wholeArray->length));temp++, i++);
	
	
	return i;
}
