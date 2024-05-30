//					implementation dependencies
#include "interface.h"
#include <stdio.h>
#include <string.h>
#include "../../lib/cbuffer/interface.h"

/*
* using static functions and static global variables 
* we hide implementation details from other modules
*/
static void *getItem(void *vitems, size_t i){
	Item *items = vitems;
	return &items[i];
}

static yn_t matchItem(void *vitems, size_t i, void *item){
	Item *items = vitems;
	if(&items[i] == item){
		return YES;
	}
	return NO;
}

static Item *getItemByName(List *self, const char *name){
	if(name == NULL) return NULL;
	for(Item *item=self->first; item!=NULL; item=item_getNext(item)){
		if(strncmp(item_getName(item), name, item_getNameSize(item)) == 0){
			return item;
		}
	}
	return NULL;
}

//					parts of this module
#include "icommand.c"

result_t list_begin(List *self){
	size_t initial_alloc_items_count = 5;
	size_t initial_alloc_mem_blocks_count = 1; 
	size_t alloc_items_increment = 10; 
	size_t alloc_items_count_max = 200;
	stime_t min_time_between_allocs = usToStime(2000);
	size_t item_size = sizeof(Item);
	if(buffer_begin(&self->buffer, initial_alloc_items_count, initial_alloc_mem_blocks_count, alloc_items_increment, alloc_items_count_max, min_time_between_allocs, item_size, getItem, matchItem) != RESULT_SUCCESS){
		fprintf(stderr, "list_begin(): buffer begin failed\n");
		return RESULT_FAILURE;
	}
	self->first = NULL;
	self->last = NULL;
	iCommandACDP *ic_acdp = &self->icommand_acdp;
	iCommand *ic = &self->icommand;
	iCommandACDP_setParam(ic_acdp, self, onAddCommand, onClearCommand, onDeleteCommand, onPrintCommand);
	iCommandACDP_buildCommonInterface(ic_acdp, ic);
	node_init(&self->node);
	node_setData(&self->node, ic);
	return RESULT_SUCCESS;
}

void list_setName(List *self, const char *name){
	node_setName(&self->node, name);
}

Node *list_getRootNode(List *self){
	return &self->node;
}

void list_control(List *self){
	for(Item *item=self->first; item!=NULL; item=item_getNext(item)){
		item_control(item);
	}
}

void list_free(List *self){
	buffer_free(&self->buffer);
}

