#include "core.h"

void Build_derived_type(border* indata, MPI_Datatype* message_type_ptr){
  int block_lengths[3];

  MPI_Aint displacements[3];
  MPI_Aint addresses[4];
  MPI_Datatype typelist[3];

  /* Создает производный тип данных, содержащий три int */

  /* Сначала нужно определить типы элементов */

  typelist[0]=MPI_INT;
  typelist[1]=MPI_INT; 
  typelist[2]=MPI_INT;

 

  /* Определить количество элементов каждого типа */
  block_lengths[0]=block_lengths[1]=block_lengths[2] = 1;
  
  /* Вычислить смещения элементов * относительно indata */
  MPI_Address(indata, &addresses[0]);
  MPI_Address(&(indata->left), &addresses[1]);
  MPI_Address(&(indata->right), &addresses[2]);
  MPI_Address(&(indata->length), &addresses[3]);

  displacements[0]=addresses[1]-addresses[0];
  displacements[1]=addresses[2]-addresses[0];
  displacements[2]=addresses[3]-addresses[0];
  
  /* Создать производный тип */
  MPI_Type_struct(3, block_lengths, displacements,typelist, message_type_ptr);
  /* Зарегистрировать его для использования */
  MPI_Type_commit(message_type_ptr);
} /* Build_derived_type */

