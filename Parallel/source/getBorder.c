#include "support.h"

void getBorder(border* borders,int N,int size,int mode,double *times){
	int h,m,curIdx=0;
	int prevrightborder=0;//предыдущая правая граница
	double R=0.;//Коэффициент для неравномерного деления
	int missing=0;//число потерянных строк
	int bordersIdx[size]/*Тут лежат порядковые номера процессов*/,buf=0/*буффер*/;
	
	for(int i=0;i<size;++i) bordersIdx[i]=i;//изначально порядок совпадает с общим порядком следования

	h=N/(size);
	m=(N%(size));
	
	switch(mode){
		case SIMPLE_BREAK/*Равномерное деление*/:
			if(m==0){
				for(int g=0;g<size;g++){
					borders[g].left=(g)*h;
					borders[g].right=(g+1)*h-1;
					borders[g].length=borders[g].right-borders[g].left+1;
				}
			}
			else{

				for(int g=0;g<m;++g){
					borders[g].left=g*(h+1);
					borders[g].right=(g+1)*h+g;
					borders[g].length=borders[g].right-borders[g].left+1;
				}

				for(int g=m;g<size;++g){
					borders[g].left=(g)*h+m;
					borders[g].right=(g)*h+h+m-1;
					borders[g].length=borders[g].right-borders[g].left+1;
				}		
			}
		break;
		case ADDAPRIVE_BREAK/*Неравномерное деление*/:
			for(int i=0;i<size;++i)	R+=1./times[i];//Считаем коэффициент
			
			for(int i=0;i<m;++i){//первоначальный расчет длинн
				borders[i].length=N*(1./(times[i]*R));
				missing+=borders[i].length;
			}
			
			for(int i=m;i<size;++i){//первоначальный расчет длинн
				borders[i].length=N*(1./(times[i]*R));
				missing+=borders[i].length;
			}
			
			missing=N-missing;//Подсчитываем число потерянных строк
			
			//Самая "быстрая" пузырьковая сортировка xD (сортируем индексы процессов в порядке производительности)
			for(int i=0;i<size;++i)
				for(int j=size-1;j>i;--j){
					if(times[bordersIdx[j]]<times[bordersIdx[j-1]]){
						buf=bordersIdx[j];
						bordersIdx[j]=bordersIdx[j-1];
						bordersIdx[j-1]=buf;
					}
				}
		    
		    //Досыпаем потерянные строки в порядке уменьшения производительности
		    for(int i=missing;i>0;--i){
				if(curIdx==size) curIdx=0;
				++(borders[bordersIdx[curIdx]]).length;
				curIdx++;
			}
			
			//Вычисляем новые границы 
			for(int i=0;i<size;++i){
				borders[i].left=prevrightborder;
				borders[i].right=prevrightborder+(borders[i].length-1);
				prevrightborder=borders[i].right+1;
			}
			
		break;
		default:
			return;
		break;
	}
}
