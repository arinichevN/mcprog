//					implementation dependencies
#include "interface.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

result_t buffer_begin(Buffer *self, size_t initial_alloc_items_count, size_t initial_alloc_mem_blocks_count, size_t alloc_items_increment, size_t alloc_items_count_max, stime_t min_time_between_allocs, size_t item_size, void *(*getItemFunction)(void *, size_t), yn_t (*matchItemFunction)(void *, size_t, void *)) {
	if(initial_alloc_items_count <= 0){
		fprintf(stderr, "buffer_begin: invalid parameter: initial_alloc_items_count\n");
		return RESULT_FAILURE;
	}
	if(initial_alloc_mem_blocks_count <= 0){
		fprintf(stderr, "buffer_begin: invalid parameter: initial_alloc_mem_blocks_count\n");
		return RESULT_FAILURE;
	}
	if(alloc_items_count_max <= 0 || alloc_items_count_max < initial_alloc_items_count){
		fprintf(stderr, "buffer_begin: invalid parameter: alloc_items_count_max\n");
		return RESULT_FAILURE;
	}
	self->initial_alloc_items_count = initial_alloc_items_count;
	self->alloc_items_increment = alloc_items_increment;
	self->alloc_items_count_max = alloc_items_count_max;
	self->min_time_between_allocs = min_time_between_allocs;
	self->alloc_items_count = self->initial_alloc_items_count;
	
	self->item_size = item_size;
	self->getItem = getItemFunction;
	self->matchItem = matchItemFunction;
	//lets allocate some memory
	self->mblocks = calloc(initial_alloc_mem_blocks_count, sizeof (*self->mblocks));
	if(self->mblocks == NULL){
		fprintf(stderr, "buffer_begin: failed to allocate memory\n");
		return RESULT_FAILURE;
	}
	self->max_count = initial_alloc_mem_blocks_count;
	
	for(size_t i=0; i<self->max_count; i++){
		result_t r = bufferMemBlock_begin(&self->mblocks[i], self->alloc_items_count, self->item_size, self->getItem, self->matchItem);
		if(r != RESULT_SUCCESS){
			for(size_t k=0; k<i; k++){
				bufferMemBlock_free(&self->mblocks[k]);
			}
			free(self->mblocks);
			self->mblocks = NULL;
			self->max_count = 0;
			return RESULT_FAILURE;
		}
	}
	
	self->last_alloc_time = getCurrentTime();
	return RESULT_SUCCESS;
}

static void *occupyEmptyItem(Buffer *self, size_t ind_from, size_t ind_to){
	for(size_t i=ind_from; i<ind_to; i++){
		BufferMemBlock *mblock = &self->mblocks[i];
		void *out = bufferMemBlock_occupyEmptyItem(mblock);
		if(out != NULL){
			return out;
		}
	}
	return NULL;
}

/*
* using static functions and static global variables 
* we hide implementation details from other modules
*/
static result_t allocNewSpace(Buffer *self){
	stime_t tp = getTimePassed(self->last_alloc_time);
#ifdef MODE_TEST
		printf("Time passed after last memory allocation: %ld(s) %ld(ns)\n", stime_getSeconds(tp), stime_getNanoseconds(tp));
		printf("Minumal time: %ld(s) %ld(ns)\n", stime_getSeconds(self->min_time_between_allocs), stime_getNanoseconds(self->min_time_between_allocs));
#endif
	if(stime_compareL(&tp, &self->min_time_between_allocs) == YES){
		self->alloc_items_count += self->alloc_items_increment;
#ifdef MODE_TEST
		printf("Time to increase number of extra items to allocate memory for: %ld\n", self->alloc_items_count);
#endif
	}
#ifdef MODE_TEST
		printf("Trying to reallocate memory for %ld more items\n", self->alloc_items_count);
#endif
	size_t new_max_count = self->max_count + 1;
	size_t new_size = sizeof (*self->mblocks) * new_max_count;
	void *new_space = realloc(self->mblocks, new_size);
	if(new_space == NULL){
		if(errno == ENOMEM){//old pointer is OK
			fprintf(stderr, "buffer_occupyItem: failed to allocate memory\n");
			return RESULT_FAILURE;
		} else {//fatal error
			self->mblocks = NULL;
			self->max_count = 0;
			fprintf(stderr, "buffer_occupyItem: failed to allocate memory, data destroyed\n");
			return RESULT_FAILURE;
		}
	}
	self->mblocks = new_space;
	for(size_t i=self->max_count; i<new_max_count; i++){
		result_t r = bufferMemBlock_begin(&self->mblocks[i], self->alloc_items_count, self->item_size, self->getItem, self->matchItem);
		if(r != RESULT_SUCCESS){
			for(size_t k=self->max_count; k<i; k++){
				bufferMemBlock_free(&self->mblocks[k]);
			}
			return RESULT_FAILURE;
		}
	}
	self->max_count = new_max_count;
	self->last_alloc_time = getCurrentTime();
#ifdef MODE_TEST
		printf("Memory reallocated for %ld total mblocks\n", self->max_count);
#endif
	return RESULT_SUCCESS;
}

void *buffer_occupyItem(Buffer *self){
	void *out = NULL;
	out = occupyEmptyItem(self, 0, self->max_count);
	if(out != NULL){
		return out;
	}

	size_t last_max_count = self->max_count;
	result_t r = allocNewSpace(self);
	if(r != RESULT_SUCCESS){
		return NULL;
	}

	out = occupyEmptyItem(self, last_max_count, self->max_count);
	return out;
}

yn_t buffer_freeItem(Buffer *self, void *item){
	for(size_t i=0; i<self->max_count; i++){
		BufferMemBlock *mblock = &self->mblocks[i];
		if(bufferMemBlock_freeItem(mblock, item) == YES){ 
			return YES;
		}
	}
	return NO;
}

void buffer_free(Buffer *self){
	for(size_t i=0; i<self->max_count; i++){
		BufferMemBlock *mblock = &self->mblocks[i];
		bufferMemBlock_free(mblock);
	}
	free(self->mblocks);
	self->mblocks = NULL;
	self->max_count = 0;
}

#ifdef MODE_TEST
void buffer_print(Buffer *self){
	printf("=======================BUFFER BEGIN=========================\n");
	printf("alloc_items_count=%ld\n", self->alloc_items_count);
	printf("Number of mem blocks: %ld\n", self->max_count);
	printf("last_alloc_time=%ld(s) %ld(ns)\n", stime_getSeconds(self->last_alloc_time), stime_getNanoseconds(self->last_alloc_time));
	for(size_t i=0; i<self->max_count; i++){
		BufferMemBlock *mblock = &self->mblocks[i];
		bufferMemBlock_print(mblock);
	}
	printf("=======================BUFFER END===========================\n");
}
#endif


