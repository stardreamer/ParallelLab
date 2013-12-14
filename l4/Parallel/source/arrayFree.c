#include "datatypes.h"

void arrayFree(array* emptyArray){
	(*emptyArray).myBorder.left=(*emptyArray).myBorder.right=(*emptyArray).myBorder.length=0;
	free((*emptyArray).Arr);
	(*emptyArray).Arr=NULL;
}
