#ifndef I_COMMAND_H
#define I_COMMAND_H

typedef enum {
	ICOMMAND_KIND_UNKNOWN,
	ICOMMAND_KIND_ACDP,
	ICOMMAND_KIND_EP,
	ICOMMAND_KIND_PSTSP
}iCommandKind_t;

/*
* do not access structure elements from other modules directly, 
* although they are declared public for simplification of memory allocation.
*/
typedef struct icommand_st iCommand;
struct icommand_st {
	void *ifc;
	iCommandKind_t kind;
};

#define ICOMMAND_INITIALIZER {.ifc = NULL, .kind = ICOMMAND_KIND_UNKNOWN}

extern void iCommand_init(iCommand *self);
extern void iCommand_setParam(iCommand *self, void *ifc, iCommandKind_t kind);

#endif




