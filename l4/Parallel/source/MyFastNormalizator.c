#include "datatypes.h"
#include <stdio.h>

int MyFastNormalizator(array* preSorted,array* pastArray){
	
	array first=ARRAY_INIT;
	array second=ARRAY_INIT;
	array result=ARRAY_INIT;
	first.Arr=preSorted->Arr;
	second.Arr=preSorted->Arr;
	int i=0,j=0,index=0;
	
	for(;first.length<(preSorted->length-1) && 
		preSorted->Arr[first.length]<preSorted->Arr[first.length+1]
		;second.Arr++,first.length++)
		;/*VOID*/
	second.Arr++;
	first.length++;
	
	
	second.length=preSorted->length-first.length;
	//fprintf(stderr,"~m %i %lf\n",second.length,second.Arr[0]);

	result.length=preSorted->length;
	result.Arr=(double *)malloc(sizeof(double)*result.length);
	
	/*for(int l=0;l<preSorted->length;l++)
		fprintf(stderr,"~e %i %lf\n",l,preSorted->Arr[l]);

	fprintf(stderr,"len %i %i %i\n",first.length,second.length,result.length);*/
	while (i < first.length && j < second.length){
        if (first.Arr[i] < second.Arr[j]){
            result.Arr[index] = first.Arr[i];
            i++;
        }
        else{
            result.Arr[index] = second.Arr[j];
            j++;
        }

        index++;
    }
	
	while (i < first.length){
        result.Arr[index] = first.Arr[i];
        index++;
        i++;
    }

    while (j < second.length){
        result.Arr[index] = second.Arr[j];
        index++;
        j++;
    }
	
	
	/*for(int l=0;l<result.length;l++)
		fprintf(stderr,"~e %i %lf\n",l,result.Arr[l]);
		*/
	free(pastArray->Arr);
	pastArray->Arr=result.Arr;
	preSorted->Arr=result.Arr;
	preSorted->length=result.length;

	return 0;
}
