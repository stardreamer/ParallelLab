#include "datatypes.h"

/**
 * Возвращает структуру хранящую индекс
 * первого элемента большего, чем ведущий  
 * @param wholeArray указатель на массив
 * @param lider ведущий элемент
 * @return  временный ниндзя
 * @author Arthur Asylgareev (Virid Raven)
 * @see #ninja
 */
 
ninja getNinja(array* wholeArray, double lider){
	ninja tempNinja = NINJA_INIT;	
	//получаем индекс первого элемента большего чем ведущий
	tempNinja.ninjaIdx=getNinjaIdx(wholeArray,lider);
	//получаем длину хвоста
	tempNinja.ninjaBlade=wholeArray->length-tempNinja.ninjaIdx;
	
	return tempNinja;
}
