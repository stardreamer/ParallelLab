#include "datatypes.h"

ninja getNinja(array* wholeArray, double lider){
	ninja tempNinja = NINJA_INIT;
	
	//получаем индекс первого элемента большего чем ведущий
	tempNinja.ninjaIdx=getNinjaIdx(wholeArray,lider);
	//получаем указатель на этот элемент
	tempNinja.ninjaPointer=&(*wholeArray).Arr[tempNinja.ninjaIdx];
	tempNinja.ninjaBlade=(*wholeArray).myBorder.length-tempNinja.ninjaIdx;
	
	return tempNinja;
}
