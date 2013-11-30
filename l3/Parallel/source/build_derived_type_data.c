#include "core.h"


void Build_derived_type_data(point* indata, MPI_Datatype* message_type_ptr){
  int block_lengths[4];

  MPI_Aint displacements[4];
  MPI_Aint addresses[5];
  MPI_Datatype typelist[4];

  /* Создает производный тип данных, содержащий три int */

  /* Сначала нужно определить типы элементов */

  typelist[0]=MPI_DOUBLE;
  typelist[1]=MPI_DOUBLE; 
  typelist[2]=MPI_DOUBLE;
  typelist[3]=MPI_DOUBLE;
 

  /* Определить количество элементов каждого типа */
  block_lengths[0]=block_lengths[1]=block_lengths[2]=block_lengths[3] = 1;
  
  /* Вычислить смещения элементов * относительно indata */
  MPI_Address(indata, &addresses[0]);
  MPI_Address(&(indata->X), &addresses[1]);
  MPI_Address(&(indata->Y), &addresses[2]);
  MPI_Address(&(indata->U), &addresses[3]);
  MPI_Address(&(indata->T), &addresses[4]);

  displacements[0]=addresses[1]-addresses[0];
  displacements[1]=addresses[2]-addresses[0];
  displacements[2]=addresses[3]-addresses[0];
  displacements[3]=addresses[4]-addresses[0];
  
  /* Создать производный тип */
  MPI_Type_struct(4, block_lengths, displacements,typelist, message_type_ptr);
  /* Зарегистрировать его для использования */
  MPI_Type_commit(message_type_ptr);
} /* Build_derived_type */


