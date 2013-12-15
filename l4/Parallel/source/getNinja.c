#include "datatypes.h"

ninja getNinja(array* wholeArray, double lider){
	ninja tempNinja = NINJA_INIT;	
	//получаем индекс первого элемента большего чем ведущий
	tempNinja.ninjaIdx=getNinjaIdx(wholeArray,lider);
	//получаем длину хвоста
	tempNinja.ninjaBlade=wholeArray->length-tempNinja.ninjaIdx;
	
	return tempNinja;
}
