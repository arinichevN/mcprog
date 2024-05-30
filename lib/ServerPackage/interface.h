#ifndef SERVER_PACKAGE_H
#define SERVER_PACKAGE_H

//					interface dependencies
#include <stddef.h>
#include "../result_t/interface.h"

/*
* do not access structure elements from other modules directly, 
* although they are declared public for simplification of memory allocation.
*/
typedef struct serverpackage_st ServerPackage;
struct serverpackage_st {
	char *items;
	size_t items_count;
	size_t buffer_size;
};

extern result_t serverPackage_begin(ServerPackage *self, size_t items_count);
extern char *serverPackage_getCharBuffer(ServerPackage *self);
extern size_t serverPackage_getCharBufferSize(ServerPackage *self);
extern size_t serverPackage_getCharBufferLength(ServerPackage *self);
extern void serverPackage_clear(ServerPackage *self);
extern void serverPackage_free(ServerPackage *self);
#ifdef MODE_TEST
extern void serverPackage_print(ServerPackage *self);
#endif

#endif




