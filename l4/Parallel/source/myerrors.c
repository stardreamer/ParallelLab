#include "myerrors.h"

#define SORT_SUC_STR "Sort successed!\n"
#define SORT_UNSUC_STR "Sort not successed!\n"
#define UNKNOWN_ERROR "Unknown error occured!\n"

const char* errorString(int errorcode){
	switch(errorcode){
		case SORT_SUCCESSED:
			return SORT_SUC_STR;
		break;
		
		case SORT_UNSUCCESSED:
			return SORT_UNSUC_STR;
		break;
		
		default:
			return UNKNOWN_ERROR;
		break;
	}
}
