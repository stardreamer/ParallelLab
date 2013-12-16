ParallelLab
===========
Лабораторные работы по дисциплине "Параллельное программирование"

Примеры запуска:
Л1:
     mpirun -np 2 mpi 100
Л2:
    make -f Makefile.mak clean mpiMatrix && mpirun -np 2 ../bin/mpiMatrix 100
Л3:
    make -f Makefile.mak clean mpiConductivity && mpirun -np 2 ../bin/mpiConductivity 10. 100 100 && python3 ../rendering/render.py
Л4:
    make -f Makefile.mak clean mpiSort && mpirun -np 2 ../bin/mpiSort full 300000 42