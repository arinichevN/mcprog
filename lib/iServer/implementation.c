//					implementation dependencies
#include "interface.h"

void iServer_setParam(iServer *self, result_t (*begin_function)(serveFunc_t), void (*greeting_function)(), void (*control_function)(), void (*free_function)()){
	self->begin = begin_function;
	self->greeting = greeting_function;
	self->control = control_function;
	self->free = free_function;
}

result_t iServer_begin(iServer *self, serveFunc_t serve_func){
	return self->begin(serve_func);
}

void iServer_greeting(iServer *self){
	self->greeting();
}

void iServer_control(iServer *self){
	self->control();
}

void iServer_free(iServer *self){
	self->free();
}

