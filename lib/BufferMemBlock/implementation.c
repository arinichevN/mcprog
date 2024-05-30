//					implementation dependencies
#include "interface.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
* using static functions and static global variables 
* we hide implementation details from other modules
*/
static void resetItems(BufferMemBlock *self){
	for(size_t i=0; i<self->count; i++){
		self->states[i] = BIS_EMPTY;
	}
}

result_t bufferMemBlock_begin(BufferMemBlock *self, size_t alloc_items_count, size_t item_size, void *(*getItemFunction)(void *, size_t), yn_t (*matchItemFunction)(void *, size_t, void *)) {
	if(alloc_items_count <= 0){
		fprintf(stderr, "bufferMemBlock_begin: invalid parameter: alloc_items_count\n");
		return RESULT_FAILURE;
	}
	if(item_size <= 0){
		fprintf(stderr, "bufferMemBlock_begin: invalid parameter: item_size\n");
		return RESULT_FAILURE;
	}
	self->getItem = getItemFunction;
	self->matchItem = matchItemFunction;
	//lets allocate some memory
	self->items = calloc(alloc_items_count, item_size);
	if(self->items == NULL){
		fprintf(stderr, "bufferMemBlock_begin: failed to allocate memory for items\n");
		return RESULT_FAILURE;
	}
	self->states = calloc(alloc_items_count, sizeof *self->states);
	if(self->states == NULL){
		fprintf(stderr, "bufferMemBlock_begin: failed to allocate memory for states\n");
		free(self->items);
		return RESULT_FAILURE;
	}
	self->count = alloc_items_count;
	resetItems(self);
	memset(self->items, 0, alloc_items_count * item_size);
	return RESULT_SUCCESS;
}

void *bufferMemBlock_occupyEmptyItem(BufferMemBlock *self){
	for(size_t i=0; i<self->count; i++){
		if(self->states[i] == BIS_EMPTY){
			self->states[i] = BIS_FULL;
			return self->getItem(self->items, i);
		}
	}
	return NULL;
}

yn_t bufferMemBlock_freeItem(BufferMemBlock *self, void *item){
	for(size_t i=0; i<self->count; i++){
		if(self->matchItem(self->items, i, item) == YES){ 
			self->states[i] = BIS_EMPTY;
			return YES;
		}
	}
	return NO;
}

void bufferMemBlock_free(BufferMemBlock *self){
	free(self->items);
	self->items = NULL;
	free(self->states);
	self->states = NULL;
	self->count = 0;
}

#ifdef MODE_TEST
void bufferMemBlock_print(BufferMemBlock *self){
	printf("----------------MEM_BLOCK BEGIN-----------------------------\n");
	for(size_t i=0; i<self->count; i++){
		switch(self->states[i]){
			case BIS_EMPTY: printf("O"); break;
			case BIS_FULL: printf("#"); break;
			default: printf("?"); break;
		}
	}
	printf("\n----------------MEM_BLOCK END-------------------------------\n");
}
#endif

