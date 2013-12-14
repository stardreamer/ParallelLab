#include "core.h"

/*
 * 
 * name: MyMpiSwap
 * @param myArray указатель на массив
 * @param ninjaIdx индекс ведущего элемента
 * @param inlen количество принимаемых элементов
 * @param outlen количество отправляемых элементов
 * @param rank номер процесса
 * @param ProcNum общее число процессов
 * @param currentComm коммуникатор
 * @return void
 * 
 */
void MyMpiSwap(array* myArray, long long int ninjaIdx,int inlen,int outlen, int rank,int ProcNum, MPI_Comm currentComm){
	array bufArray=ARRAY_INIT; //Здесь будет храниться указатель на часть полученную с другого процесса. 

	
	if(inlen!=0){ //Все процессы, которым нужно принимать данные - выделяют память. Важно - проверка на NULL. Временами массивы становятся пустым			
			bufArray.length=inlen; //находим длину нового хвоста
			if(myArray->Arr!=NULL)
				myArray->Arr=(double *)realloc(myArray->Arr,(bufArray.length+myArray->length)*sizeof(double));
			else
				myArray->Arr=(double *)malloc((bufArray.length+myArray->length)*sizeof(double));
				
			bufArray.Arr=&(myArray->Arr[myArray->length]); // получем указатель на хвост. Выхода за пределы массива не происходит ибо память довыделена
	}	
	
	/*
	 *Если процессу нужно передавать данные, то смотрим на его номер. 
	 *Если номер в первой половине, то отдаем элементы большие ведущего
	 *Если номер во второй половине, то отдаем элементы меньшие ведущего
	 *Если процессу нужно принимать данные, то смотрим на его номер
	 *Если номер в первой половине, то принимаем с обрубка хвоста массива
	 *Если номер во второй половине, то нужны дополнительные телодвижения
	 * 
	 * */
	if(rank<(ProcNum/2)){
		if(outlen!=0)
			MPI_Send(&(myArray->Arr[ninjaIdx]), outlen,MPI_DOUBLE,rank+(ProcNum/2),0,currentComm);
	}
	else{	
		if(inlen!=0)
			MPI_Recv(bufArray.Arr,inlen,MPI_DOUBLE,rank-(ProcNum/2),0,currentComm,MPI_STATUS_IGNORE);	
	}
	
	if(rank>=(ProcNum/2)){
		if(outlen!=0)
			MPI_Send(myArray->Arr, outlen, MPI_DOUBLE, rank-(ProcNum/2), 0, currentComm);	
	}
	else{
		if(inlen!=0)
			MPI_Recv(bufArray.Arr,inlen,MPI_DOUBLE,rank+(ProcNum/2),0,currentComm,MPI_STATUS_IGNORE);
	}
	
	
	
	if(outlen!=0){	
		if(inlen==0){ // если не принимаем данные то надо осторожно поработать с памятью
			myArray->length=(myArray->length-outlen); // для начала вычислим длину массива
			
			if(myArray->length==0){ // если передаем весь массив, то realloc не справится и надо освобождать память вручную
				free(myArray->Arr);
				myArray->Arr=NULL;
			}
			else{ 
				if(rank<(ProcNum/2)) // для первой половины достаточно просто отрезать хвост
					myArray->Arr=(double *)realloc(myArray->Arr,myArray->length*sizeof(double));
				else{ // для второй половины надо сначала перенести оставшиеся элементы в начало массива
					memcpy(myArray->Arr, &(myArray->Arr[ninjaIdx]), sizeof(double)*(myArray->length));
					myArray->Arr=(double *)realloc(myArray->Arr,myArray->length*sizeof(double));
				}
			}
		}
	}


	if(inlen!=0){		
		if(rank<(ProcNum/2)) // Хитрая часть. Если процесс в первой половине, то просто вживляем хвост на место старого
			memcpy(&(myArray->Arr[ninjaIdx]), bufArray.Arr, sizeof(double)*bufArray.length);
		else{ // иначе приходится снача перенести в начало массива оставшиеся элементы
			memcpy(myArray->Arr, &(myArray->Arr[ninjaIdx]), sizeof(double)*(myArray->length-outlen));
			memcpy(&(myArray->Arr[myArray->length-outlen]), bufArray.Arr, sizeof(double)*bufArray.length);
		}
		myArray->length=(inlen+myArray->length-outlen);
		myArray->Arr=(double *)realloc(myArray->Arr,myArray->length*sizeof(double)); //хвост опять обрубаем
	}
}
