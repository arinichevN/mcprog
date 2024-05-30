#ifndef I_COMMAND_ACDP_H
#define I_COMMAND_ACDP_H

//					interface dependencies
#include <stddef.h>
#include "../common/interface.h"

/*
* do not access structure elements from other modules directly, 
* although they are declared public for simplification of memory allocation.
*/
typedef struct icommandacdp_st iCommandACDP;
struct icommandacdp_st {
	void *slave;
	void (*add)(void *, const char *);
	void (*clear)(void *);
	void (*delete)(void *, const char *);
	void (*print)(void *, char *, size_t);
};

extern void iCommandACDP_init(iCommandACDP *self);
extern void iCommandACDP_setParam(iCommandACDP *self, void *slave, void (*add)(void *, const char *), void (*clear)(void *), void (*delete)(void *, const char *), void (*print)(void *, char *, size_t));
extern void iCommandACDP_add(iCommandACDP *self, const char *name_str);
extern void iCommandACDP_clear(iCommandACDP *self);
extern void iCommandACDP_delete(iCommandACDP *self, const char *name_str);
extern void iCommandACDP_print(iCommandACDP *self, char *buf, size_t buf_size);
extern void iCommandACDP_buildCommonInterface(iCommandACDP *self, iCommand *common_ifc);

#endif




