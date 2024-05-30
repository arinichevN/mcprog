#ifndef APP_LIST_H
#define APP_LIST_H

//					interface dependencies
#include "../../lib/result_t/interface.h"
#include "../../lib/Node/interface.h"
#include "../../model/List/interface.h"

extern result_t appList_begin();
extern Node *appList_getRootNode();
extern void appList_control();
extern void appList_free();

#endif




