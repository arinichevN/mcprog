//					implementation dependencies
#include "interface.h"

/*
* using static functions and static global variables 
* we hide implementation details from other modules
*/
static List list_n;
static List *list = &list_n;

result_t appList_begin(){
	if(list_begin(list) != RESULT_SUCCESS){
		return RESULT_FAILURE;
	}
	list_setName(list, "list");
	return RESULT_SUCCESS;
}

Node *appList_getRootNode(){
	return list_getRootNode(list);
}

void appList_control(){
	list_control(list);
}

void appList_free(){
	list_free(list);
}

