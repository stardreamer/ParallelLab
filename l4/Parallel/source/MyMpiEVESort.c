#include "core.h"

/**
 * Организует параллельную чет-неченую сортировку массива 
 *
 * @param myArray  указатель на массив
 * @author Arthur Asylgareev (Virid Raven)
 * @return  число шагов
 * 
 */

int MyMpiEVESort(array* myArray){
	int ProcNum=0,rank=0,color=0,numofiter=0;
	//ninja myNinja=NINJA_INIT;
	MPI_Comm currentComm = MPI_COMM_WORLD;
	MPI_Comm_rank(currentComm,&rank);		
	MPI_Comm_size(currentComm,&ProcNum);
	numofiter=ProcNum;
	//Сортировка(пузырек)
	MyBubbleSort(myArray);
	
	for(int i=0; i<numofiter;i++){
		
		currentComm = MPI_COMM_WORLD;
		MPI_Comm_rank(currentComm,&rank);		

		if(i%2==0){
			if(rank%2==0)
				color=rank;
			else
				color=rank-1;
		}
		else{
			if(rank%2!=0)
				color=rank;
			else{
				if(rank!=0)
					color=rank-1;

			}
		}
		
		
							
		MPI_Comm_split(currentComm,color,rank,&currentComm);
		MPI_Comm_rank(currentComm,&rank);		
		MPI_Comm_size(currentComm,&ProcNum);
		/*myNinja.ninjaIdx=myArray->length/2;
		myNinja.ninjaBlade=myNinja.ninjaIdx;*/
		//Обмениваемся данными и сливаем массивы
		if(ProcNum!=1)
			MyEveSwapAndMerge(myArray, rank, currentComm);

	}
      
	return numofiter;
}
