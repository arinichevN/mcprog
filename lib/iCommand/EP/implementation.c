//					implementation dependencies
#include "interface.h"
#include <stddef.h>

//					parts of this module
#include "../dummy_functions/exit.c"
#include "../dummy_functions/print.c"

void iCommandEP_init(iCommandEP *self){
	self->slave = NULL;
	self->exit = exit_DUMMY;;
	self->print = print_DUMMY;
}

void iCommandEP_setParam(iCommandEP *self, void *slave, void (*exit)(void *), void (*print)(void *, char *, size_t)){
	self->slave = slave;
	self->exit = exit;
	self->print = print;
}

void iCommandEP_exit(iCommandEP *self){
	self->exit(self->slave);
}

void iCommandEP_print(iCommandEP *self, char *buf, size_t buf_size){
	self->print(self->slave, buf, buf_size);
}

void iCommandEP_buildCommonInterface(iCommandEP *self, iCommand *common_ifc){
	iCommand_setParam(common_ifc, self, ICOMMAND_KIND_EP);
}
