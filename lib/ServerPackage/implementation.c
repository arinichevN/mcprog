//					implementation dependencies
#include "interface.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


result_t serverPackage_begin(ServerPackage *self, size_t items_count){
	size_t item_size = sizeof *self->items;
	void *buffer = calloc(items_count, item_size);
	if(buffer == NULL){
		fprintf(stderr, "serverPackage_begin: failed to allocate memory\n");
		return RESULT_FAILURE;
	}
	self->items = buffer;
	self->items_count = items_count;
	self->buffer_size = item_size * self->items_count;
#ifdef MODE_TEST
	printf("serverPackage_begin: items_count=%ld buffer_size=%ld\n", self->items_count, self->buffer_size);
#endif
	return RESULT_SUCCESS;
}

char *serverPackage_getCharBuffer(ServerPackage *self){
	return self->items;
}

size_t serverPackage_getCharBufferSize(ServerPackage *self){
	return self->buffer_size - 1;
}

size_t serverPackage_getCharBufferLength(ServerPackage *self){
	return self->items_count - 1;
}

void serverPackage_clear(ServerPackage *self){
	memset(self->items, 0, self->buffer_size);
}

void serverPackage_free(ServerPackage *self){
	free(self->items);
	self->items = NULL;
	self->items_count = 0;
	self->buffer_size = 0;
}


#ifdef MODE_TEST
void serverPackage_print(ServerPackage *self){
	printf("\n\nServerPackage BEGIN\n");
	printf("items_count=%ld\n", self->items_count);
	printf("buffer_size=%ld\n", self->buffer_size);
	printf("%s", self->items);
	printf("\nServerPackage END\n");
}
#endif
