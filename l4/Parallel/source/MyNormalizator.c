 #include "datatypes.h"

/*
 * 
 * name: MyNormalizator
 * @param firstArray указатель на массив
 * @return
 * 
 */
int MyNormalizator(array* firstArray){
	int sentinel=0; //индекс первого неотсортированного элемента
	double buf=0.;
	int counter=0; //число переcтановок

	if(firstArray->Arr!=NULL){
	for(;sentinel<(firstArray->length-1) && 
			firstArray->Arr[sentinel]<firstArray->Arr[sentinel+1]
		;sentinel++)
		; /*VOID*/

	for(sentinel++;sentinel < firstArray->length;sentinel++){
		for(int j=sentinel-1;j>=0;j--){
			if(firstArray->Arr[j+1]<firstArray->Arr[j]){
				counter++;
				buf=firstArray->Arr[j+1];
				firstArray->Arr[j+1]=firstArray->Arr[j];
				firstArray->Arr[j]=buf;
			}
			else
				break;
		}
	}
	}
	return counter;
}
