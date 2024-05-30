//					implementation dependencies
#include "interface.h"
#include <stddef.h>

//					parts of this module
#include "../dummy_functions/print.c"
#include "../dummy_functions/start.c"
#include "../dummy_functions/stop.c"

void iCommandPStSp_init(iCommandPStSp *self){
	self->slave = NULL;
	self->print = print_DUMMY;
	self->start = start_DUMMY;
	self->stop = stop_DUMMY;
}

void iCommandPStSp_setParam(iCommandPStSp *self, void *slave, void (*print)(void *, char *, size_t), void (*start)(void *), void (*stop)(void *)){
	self->slave = slave;
	self->print = print;
	self->start = start;
	self->stop = stop;
}

void iCommandPStSp_print(iCommandPStSp *self, char *buf, size_t buf_size){
	self->print(self->slave, buf, buf_size);
}

void iCommandPStSp_start(iCommandPStSp *self){
	self->start(self->slave);
}

void iCommandPStSp_stop(iCommandPStSp *self){
	self->stop(self->slave);
}

void iCommandPStSp_buildCommonInterface(iCommandPStSp *self, iCommand *common_ifc){
	iCommand_setParam(common_ifc, self, ICOMMAND_KIND_PSTSP);
}
