#ifndef MY_ERRORS_H
#define MY_ERRORS_H

#define SORT_SUCCESSED 0
#define SORT_UNSUCCESSED -1 
#define WRONG_PROC_NUMBER -2
#define LOCAL_SORT_UNSUCCESSED -3

extern int myerror; 

const char* errorString(int errorcode);



#endif
