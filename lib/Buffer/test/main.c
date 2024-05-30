#include <stdlib.h>
#include <stdio.h>
#include "../interface.h"

Buffer nob;
Buffer *ob = &nob;

typedef struct {
	int id;
	int e2;
	char e3;
} BufferItem;

void bitem_setId(BufferItem *self, int v){
	self->id = v;
}

int bitem_getId(BufferItem *self){
	return self->id;
}

void *getItem(void *vitems, size_t i){
	BufferItem *items = vitems;
	return &items[i];
}

yn_t matchItem(void *vitems, size_t i, void *item){
	BufferItem *items = vitems;
	if(&items[i] == item){
		return YES;
	}
	return NO;
}

 
void occupy(BufferItem **items, size_t count){
	for(size_t i=0; i<count; i++){
		items[i] = buffer_occupyItem(ob);
		if(items[i] == NULL){
			fprintf(stderr, "occupy(%ld): failed\n", i);
			return;
		}
		bitem_setId(items[i], i);
	}
}

void freeSpace(BufferItem **items, size_t count){
	for(size_t i=0; i<count; i++){
		yn_t r = buffer_freeItem(ob, items[i]);
		if(r != YES){
			fprintf(stderr, "free(%ld): failed\n", i);
			return;
		}
	}
}

void test1(size_t count){
	BufferItem *items[count];
	buffer_print(ob);
	occupy(items, count);
	printf("\n==========ITEMS:==========================================\n");
	for(size_t i=0; i<count; i++){
		BufferItem *item = items[i];
		int id = bitem_getId(item);
		printf("%d ", id);
	}
	printf("\n==========END_ITEMS==========================================\n");
	buffer_print(ob);
	freeSpace(items, count);
	buffer_print(ob);
	puts("\n\n\n////////////////////////SECOND ATTEMPT///////////////////////\n");
	occupy(items, count);
	buffer_print(ob);
	freeSpace(items, count);
	buffer_print(ob);
}

int main() {
	size_t initial_alloc_items_count = 15;
	size_t initial_alloc_mem_blocks_count = 3;
	size_t alloc_items_increment = 50;
	size_t alloc_items_count_max = 170;
	size_t item_size = sizeof (BufferItem);
	
	stime_t min_time_between_allocs = usToStime(2000);
	 
	result_t r = buffer_begin(ob, initial_alloc_items_count, initial_alloc_mem_blocks_count, alloc_items_increment, alloc_items_count_max, min_time_between_allocs, item_size, getItem, matchItem);
	if(r != RESULT_SUCCESS){
		return (EXIT_FAILURE);
	}
	
	test1(1200);
	
	buffer_free(ob);
	
	return (EXIT_FAILURE);
}


