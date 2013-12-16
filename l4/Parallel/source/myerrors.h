#ifndef MY_ERRORS_H
#define MY_ERRORS_H

#define SORT_SUCCESSED 0
#define SORT_UNSUCCESSED -1 
#define WRONG_PROC_NUMBER -2
#define LOCAL_SORT_UNSUCCESSED -3
#define UNKNOWN_MODE -4
#define UNABLE_TO_ALLOCATE_MEMORY -5

extern int myerror; /**<код ошибки*/

const char* errorString(int errorcode); /**<строковое представление ошибки*/



#endif
