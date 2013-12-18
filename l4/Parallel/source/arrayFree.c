#include "datatypes.h"

/**
 * 
 * Освобождает память выделенную под массив
 * @param fullArray указатель на массив
 * @author Arthur Asylgareev (Virid Raven)
 * @see #array
 * 
 */

void arrayFree(array* fullArray){
	fullArray->length=0;
	free(fullArray->Arr);
	fullArray->Arr=NULL;
}
