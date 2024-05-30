#ifndef LIST_H
#define LIST_H

//					interface dependencies
#include <stddef.h>
#include "../../lib/Node/interface.h"
#include "../../lib/Buffer/interface.h"
#include "../../lib/iCommand/common/interface.h"
#include "../../lib/iCommand/ACDP/interface.h"
#include "../../model/Item/interface.h"

/*
* do not access structure elements from other modules directly, 
* although they are declared public for simplification of memory allocation.
*/
typedef struct list_st List;
struct list_st {
	Buffer buffer;//here we store all items
	Item *first;
	Item *last;
	Node node;
	iCommandACDP icommand_acdp;
	iCommand icommand;
};

extern result_t list_begin(List *self);
extern void list_setName(List *self, const char *name);
extern Node *list_getRootNode(List *self);
extern void list_control(List *self);
extern void list_free(List *self);

#endif
