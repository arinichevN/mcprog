//					parts of this module
#include "interface.h"
#include "../dummy_functions/add.c"
#include "../dummy_functions/clear.c"
#include "../dummy_functions/delete.c"
#include "../dummy_functions/print.c"

void iCommandACDP_init(iCommandACDP *self){
	self->slave = NULL;
	self->add = add_DUMMY;
	self->clear = clear_DUMMY;
	self->delete = delete_DUMMY;
	self->print = print_DUMMY;
}

void iCommandACDP_setParam(iCommandACDP *self, void *slave, void (*add)(void *, const char *), void (*clear)(void *), void (*delete)(void *, const char *), void (*print)(void *, char *, size_t)){
	self->slave = slave;
	self->add = add;
	self->clear = clear;
	self->delete = delete;
	self->print = print;
}

void iCommandACDP_add(iCommandACDP *self, const char *name_str){
	self->add(self->slave, name_str);
}

void iCommandACDP_clear(iCommandACDP *self){
	self->clear(self->slave);
}

void iCommandACDP_delete(iCommandACDP *self, const char *name_str){
	self->delete(self->slave, name_str);
}

void iCommandACDP_print(iCommandACDP *self, char *buf, size_t buf_size){
	self->print(self->slave, buf, buf_size);
}

void iCommandACDP_buildCommonInterface(iCommandACDP *self, iCommand *common_ifc){
	iCommand_setParam(common_ifc, self, ICOMMAND_KIND_ACDP);
}
