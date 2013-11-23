#define typeofmatrix double
#define seed 1
#define fMin -1.
#define fMax 1.
#define reductor(i,j,nu) (((i<nu) && (j<nu)) ? (nu*i+j) : -1)

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int n=10;

double fRand(){
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

int main(int argc, char *argv[]){
	clock_t t;//Время
	typeofmatrix *a,*b,*c,sum,norm,abuf;//Указатели на матрицы и буферные переменные
	int N1,N2,N3;//Размеры матриц

	n= argc > 1 ? atoi(argv[1]):10;//Получаем размер матрицы. По умолчанию 10.
	N1=n*n;
	N2=2*N1;
	N3=3*N1;

	a=(typeofmatrix*)malloc(sizeof(typeofmatrix)*N3);//Выделяем память
	b=a+N1;//Получаем указатель на матрицу b
	c=a+N2;//Получаем указатель на матрицу с
	for(int i=0;i<N3;i++) a[i]=0.;//Зануляем

	srand(seed);//Инициализируем генератор случайных чисел
	for(int i=0;i<N2;++i)	a[i]=fRand();//Заполняем матрицы псевдослучайными числами
	
	t=clock();//Текущее время
	//Перемножаем матрицы
	for(int i=0;i<n;++i){
		for(int k=0;k<n;k++){
			abuf=a[reductor(i,k,n)];
			for(int j=0;j<n;++j){
				c[reductor(i,j,n)]+=abuf*b[reductor(k,j,n)];
			}
		}
	}
	
	//Считаем норму
	sum=0.;
	for(int i=0;i<n;i++){
		sum+=c[i]*c[i];
	}
	norm=sqrt(sum);
	
	//Время работы программы
	t=clock()-t;
	
	//Результат
	printf ("It took me %d clicks (%f seconds) norm %lf .\n",(int)t,((float)t)/CLOCKS_PER_SEC,norm);
	
	return 0;
}
