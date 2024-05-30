//					implementation dependencies
#include "interface.h"
#include <stddef.h>

void iCommand_init(iCommand *self){
	self->ifc = NULL;
	self->kind = ICOMMAND_KIND_UNKNOWN;
}

void iCommand_setParam(iCommand *self, void *ifc, iCommandKind_t kind){
	self->ifc = ifc;
	self->kind = kind;
}
