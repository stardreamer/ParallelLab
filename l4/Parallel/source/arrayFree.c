#include "datatypes.h"

/*
 * 
 * name: arrayFree
 * @param fullArray указатель на массив
 * @return void
 * 
 */

void arrayFree(array* fullArray){
	(*fullArray).length=0;
	free((*fullArray).Arr);
	(*fullArray).Arr=NULL;
}
