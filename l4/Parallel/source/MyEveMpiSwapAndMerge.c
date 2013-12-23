#include "core.h"

/**
 * 
 * бмен и слияние массивов между процессами
 * при чет-нечетной сортировке
 * @param myArray указатель на массив
 * @param rank номер процессса
 * @param currentComm коммуникатор
 * @author Arthur Asylgareev (Virid Raven)
 * @see #MyMpiEVESort()
 * 
 */

void MyEveSwapAndMerge(array* myArray,int rank, MPI_Comm currentComm){

		int inlen = 0;

		//Получаем размер отправляемого блока
		if(rank==0){
			MPI_Send(&(myArray->length),1,MPI_INT,1,0,currentComm);
			MPI_Recv(&inlen,1,MPI_INT,1,0,currentComm,MPI_STATUS_IGNORE);
		}
		else{
			MPI_Recv(&inlen,1,MPI_INT,0,0,currentComm,MPI_STATUS_IGNORE);
			MPI_Send(&(myArray->length),1,MPI_INT,0,0,currentComm);
		}

		if(inlen!=0){ //Все процессы, которым нужно принимать данные - выделяют память. Важно - проверка на NULL. Временами массивы становятся пустым			
			array tempArr=ARRAY_INIT;
			tempArr.length=myArray->length;
			
			if(myArray->Arr!=NULL)
				myArray->Arr=(double *)realloc(myArray->Arr,(inlen+myArray->length)*sizeof(double));
			else
				myArray->Arr=(double *)malloc((inlen+myArray->length)*sizeof(double));
			
		
			tempArr.length+=inlen;
			tempArr.Arr=myArray->Arr;

			if(rank==0){
				MPI_Send(myArray->Arr,myArray->length,MPI_DOUBLE,1,0,currentComm);
				MPI_Recv(&(myArray->Arr[myArray->length]),inlen,MPI_DOUBLE,1,0,currentComm,MPI_STATUS_IGNORE);
			}
			else{
				MPI_Recv(&(myArray->Arr[myArray->length]),inlen,MPI_DOUBLE,0,0,currentComm,MPI_STATUS_IGNORE);
				MPI_Send(myArray->Arr,myArray->length,MPI_DOUBLE,0,0,currentComm);
			}
				
			//MyNormalizator(&tempArr);
			MyFastNormalizator(&tempArr,myArray);
			//Меньший процесс оставляет левую половину, старший правую 
			if(rank==0){
				tempArr.Arr=(double *)realloc(tempArr.Arr,(tempArr.length/2)*sizeof(double));
				myArray->length=tempArr.length/2;
			}
			else{
				if(tempArr.length%2==0){
					memmove(tempArr.Arr, &(tempArr.Arr[tempArr.length/2]), sizeof(double)*(tempArr.length/2));
					tempArr.Arr=(double *)realloc(tempArr.Arr,(tempArr.length/2)*sizeof(double));
					myArray->length=tempArr.length/2;
				}
				else{
					memmove(myArray->Arr, &(tempArr.Arr[tempArr.length/2]), sizeof(double)*(tempArr.length/2+1));
					tempArr.Arr=(double *)realloc(tempArr.Arr,(tempArr.length/2+1)*sizeof(double));
					myArray->length=tempArr.length/2+1;
				}
				
			}
			
							
			if(tempArr.Arr!=NULL)
				myArray->Arr=tempArr.Arr;
			else{
					myerror=UNABLE_TO_ALLOCATE_MEMORY;
					return;
				}
		}
		
		
}
