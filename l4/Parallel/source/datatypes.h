#ifndef DATA_TYPE_H
#define DATA_TYPE_H
#define ARRAY_INIT {{0, 0, 0}, NULL}

typedef struct Border border;
typedef struct Array array;

struct Border{
	long long int left,right,length;
};

struct Array{
	border myBorder;
	double* Arr;
};


#endif
