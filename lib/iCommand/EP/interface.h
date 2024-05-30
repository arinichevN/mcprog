#ifndef I_COMMAND_EP_H
#define I_COMMAND_EP_H

//					interface dependencies
#include <stddef.h>
#include "../common/interface.h"

/*
* do not access structure elements from other modules directly, 
* although they are declared public for simplification of memory allocation.
*/
typedef struct icommandep_st iCommandEP;
struct icommandep_st {
	void *slave;
	void (*exit)(void *);
	void (*print)(void *, char *, size_t);
};

extern void iCommandEP_init(iCommandEP *self);
extern void iCommandEP_setParam(iCommandEP *self, void *slave, void (*exit)(void *), void (*print)(void *, char *, size_t));
extern void iCommandEP_exit(iCommandEP *self);
extern void iCommandEP_print(iCommandEP *self, char *buf, size_t buf_size);
extern void iCommandEP_buildCommonInterface(iCommandEP *self, iCommand *common_ifc);

#endif




