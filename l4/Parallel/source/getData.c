#include "datatypes.h"


/*
 * 
 * name: getData
 * @param slice границы
 * @param source источник данных
 * @param mode режим работы
 * @return указатель на массив
 * 
 */
double* getData(border* slice,void *source, int mode){
	double* temp=NULL; // указатель на массив
	
	temp=(double*) malloc(sizeof(double)*slice->length); // выделяем память
	
	if(temp!=NULL){ //проверка на корректность выделения памяти
		switch(mode){
			case RANDOM_MODE: //заполнение случайными числами
				if(*((int *)source) < 0){ //если seed не задан, то вычисляем его
					struct timeval t1;
					gettimeofday(&t1, NULL);
					*((int *)source)=t1.tv_usec * t1.tv_sec + clock();
				}

				srand(*((int *)source)); //инициализируем счетчик
				
				for(long long int i=0;i<slice->length;++i) //заполняем массив
					temp[i]=fRand();		
					
			break;
			
			case DEFINED_RANDOM_MODE:
				srand(*((int *)source)+slice->left); //инициализируем счетчик
				
				for(long long int i=0;i<slice->length;++i) //заполняем массив
					temp[i]=fRand();
			break;
			
			default:
			break;
		}
	}
		
	return temp;
}
