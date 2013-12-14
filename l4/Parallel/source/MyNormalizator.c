 #include "datatypes.h"

long long int MyNormalizator(array* firstArray){
	long long int sentinel=0;
	double buf=0.;
	long long int counter=0;

	if(firstArray->Arr!=NULL){
	for(;sentinel<(firstArray->length-1) && 
			firstArray->Arr[sentinel]<firstArray->Arr[sentinel+1]
		;sentinel++)
		; /*VOID*/

	for(sentinel++;sentinel < firstArray->length;sentinel++){
		for(long long int j=sentinel-1;j>=0;j--){
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
