#ifndef BUFFER_MEM_BLOCK_H
#define BUFFER_MEM_BLOCK_H

//					interface dependencies
#include <stddef.h>
#include "../../lib/yn_t/interface.h"
#include "../../lib/result_t/interface.h"

typedef enum {
	BIS_UNKNOWN,
	BIS_EMPTY,
	BIS_FULL
} bufferItemState_t;

/*
* do not access structure elements from other modules directly, 
* although they are declared public for simplification of memory allocation.
*/
typedef struct buffermemblock_st BufferMemBlock;
struct buffermemblock_st {
	void *items;
	bufferItemState_t *states;
	size_t count;
	void *(*getItem)(void *, size_t);
	yn_t (*matchItem)(void *, size_t, void *);
};

extern result_t bufferMemBlock_begin(BufferMemBlock *self, size_t alloc_items_count, size_t item_size, void *(*getItemFunction)(void *, size_t), yn_t (*matchItemFunction)(void *, size_t, void *));
extern void *bufferMemBlock_occupyEmptyItem(BufferMemBlock *self);
extern yn_t bufferMemBlock_freeItem(BufferMemBlock *self, void *item);
extern void bufferMemBlock_free(BufferMemBlock *self);

#ifdef MODE_TEST
extern void bufferMemBlock_print(BufferMemBlock *self);
#endif

#endif
