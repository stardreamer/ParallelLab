#include "myerrors.h"

#define SORT_SUC_STR "Global sort successed!"
#define SORT_UNSUC_STR "Global sort doesn't successed!"
#define LOCAL_SORT_UNSUC_STR "Local sort doesn't successed!"
#define WRONG_PROC_NUM_STR "Num of processes isn't power of 2!"
#define UNKNOWN_MODE_STR "Work mode is unknown!"
#define UNABLE_TO_ALLOC_MEM_STR "Unable to allocate memory!"
#define UNKNOWN_ERROR "Unknown error occured!"

/**
 * 
 * Преобразует код ошибки в строковое представление
 * @param errorcode код ошибки
 * @return  строковое представление ошибки
 * 
 */

const char* errorString(int errorcode){
	switch(errorcode){
		case SORT_SUCCESSED:
			return SORT_SUC_STR;
		break;
		
		case SORT_UNSUCCESSED:
			return SORT_UNSUC_STR;
		break;
		
		case WRONG_PROC_NUMBER:
			return WRONG_PROC_NUM_STR;
		break;
		
		case LOCAL_SORT_UNSUCCESSED:
			return LOCAL_SORT_UNSUC_STR;
		break;
		
		case UNKNOWN_MODE:
			return UNKNOWN_MODE_STR;
		break;
		
		case UNABLE_TO_ALLOCATE_MEMORY:
			return UNABLE_TO_ALLOC_MEM_STR;
		break;
		
		default:
			return UNKNOWN_ERROR;
		break;
	}
}
