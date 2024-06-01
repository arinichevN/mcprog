#ifndef MODULE_ITEM_H
#define MODULE_ITEM_H

//					interface dependencies
#include <stddef.h>
#include "../../lib/result_t/interface.h"
#include "../../lib/stime_t/interface.h"
#include "../../lib/Timer/interface.h"
#include "../../lib/Node/interface.h"
#include "../../lib/iCommand/common/interface.h"
#include "../../lib/iCommand/PStSp/interface.h"

//here we have definition of out Item type, so we can allocate memory
//for our Item in client modules
// - automatically,
// - statically and 
// - dynamically (using function item_new()) 
//
//If we will only declare our Item type, like this:
//typedef struct item_st Item;
//and implement it in im.c file, so in client modules
//we will not be able to allocate memory statically (because size of Item is unknown
//at compile time of client module). We will still be able to allocate 
//memory dynamically (using item_new()) and
//automatically like this:
//Item *o1 = alloc(item_getSize());

//typedef struct item_st Item;//for hiding structure elements from client modules

#define ITEM_DEFAULT_ID					0
#define ITEM_DEFAULT_START_DURATION_S		5
#define ITEM_DEFAULT_STOP_DURATION_S		5

typedef struct item_st Item;
struct item_st{
	int id;
	Timer timer;
	stime_t start_duration;
	stime_t stop_duration;
	void (*start)(Item *);
	void (*stop)(Item *);
	void (*control)(Item *);
	Node node;
	Item *next;
	Item *previous;
	iCommandPStSp icommand_pstsp;
	iCommand icommand;
};

extern size_t item_getSize();//for automatic memory allocation like this: Item *o1 = alloc(item_getSize());
extern Item *item_new();
extern void item_free(Item *self);
extern result_t item_begin(Item *self);
extern void item_start(Item *self);
extern void item_stop(Item *self);
extern void item_control(Item *self);
extern void item_setId(Item *self, int id);
extern int item_getId(Item *self);
extern void item_setName(Item *self, const char *name);
extern const char *item_getName(Item *self);
extern size_t item_getNameSize(Item *self);
extern Node *item_getNode(Item *self);
extern void item_connectToPrevious(Item *self, Item *previous);
extern void item_connectToNext(Item *self, Item *next);
extern Item *item_getNext(Item *self);
extern Item *item_getPrevious(Item *self);
extern const char *item_getStateStr(Item *self);
extern void item_print(Item *self, char *buf, size_t buf_size);

#endif
