#ifndef I_SERVER_H
#define I_SERVER_H

//					interface dependencies
#include <stddef.h>
#include "../result_t/interface.h"
#include "../server/interface.h"

/*
* do not access structure elements from other modules directly, 
* although they are declared public for simplification of memory allocation.
*/
typedef struct iserver_st iServer;
struct iserver_st {
	result_t (*begin)(serveFunc_t serve_func);
	void (*greeting)();
	void (*control)();
	void (*free)();
};

extern void iServer_setParam(iServer *self, result_t (*begin_function)(serveFunc_t), void (*greeting_function)(), void (*control_function)(), void (*free_function)());
extern result_t iServer_begin(iServer *self, serveFunc_t serve_func);
extern void iServer_greeting(iServer *self);
extern void iServer_control(iServer *self);
extern void iServer_free(iServer *self);

#endif




