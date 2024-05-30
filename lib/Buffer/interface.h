#ifndef BUFFER_H
#define BUFFER_H

//					interface dependencies
#include <stddef.h>
#include "../../lib/result_t/interface.h"
#include "../../lib/yn_t/interface.h"
#include "../../lib/stime_t/interface.h"
#include "../BufferMemBlock/interface.h"

/*
* do not access structure elements from other modules directly, 
* although they are declared public for simplification of memory allocation.
*/
typedef struct buffer_st Buffer;
struct buffer_st {
	BufferMemBlock *mblocks;
	size_t alloc_items_count;
	stime_t last_alloc_time;
	size_t max_count;
	
	size_t initial_alloc_items_count; 
	size_t alloc_items_increment;
	size_t alloc_items_count_max;
	stime_t min_time_between_allocs;
	
	size_t item_size;
	void *(*getItem)(void *, size_t);
	yn_t (*matchItem)(void *, size_t, void *);

};

extern result_t buffer_begin(Buffer *self, size_t initial_alloc_items_count, size_t initial_alloc_mem_blocks_count, size_t alloc_items_increment, size_t alloc_items_count_max, stime_t min_time_between_allocs, size_t item_size, void *(*getItemFunction)(void *, size_t), yn_t (*matchItemFunction)(void *, size_t, void *));
extern void *buffer_occupyItem(Buffer *self);
extern yn_t buffer_freeItem(Buffer *self, void *item);
extern void buffer_free(Buffer *self);

#ifdef MODE_TEST
extern void buffer_print(Buffer *self);
#endif

#endif
