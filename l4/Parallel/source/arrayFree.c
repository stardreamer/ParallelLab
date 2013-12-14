#include "datatypes.h"

void arrayFree(array* fullArray){
	(*fullArray).length=0;
	free((*fullArray).Arr);
	(*fullArray).Arr=NULL;
}
