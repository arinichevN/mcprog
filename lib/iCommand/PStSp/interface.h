#ifndef I_COMMAND_PSTSP_H
#define I_COMMAND_PSTSP_H

//					interface dependencies
#include <stddef.h>
#include "../common/interface.h"

/*
* do not access structure elements from other modules directly, 
* although they are declared public for simplification of memory allocation.
*/
typedef struct icommandpstsp_st iCommandPStSp;
struct icommandpstsp_st {
	void *slave;
	void (*print)(void *, char *, size_t);
	void (*start)(void *);
	void (*stop)(void *);
};

extern void iCommandPStSp_init(iCommandPStSp *self);
extern void iCommandPStSp_setParam(iCommandPStSp *self, void *slave, void (*print)(void *, char *, size_t), void (*start)(void *), void (*stop)(void *));
extern void iCommandPStSp_print(iCommandPStSp *self, char *buf, size_t buf_size);
extern void iCommandPStSp_start(iCommandPStSp *self);
extern void iCommandPStSp_stop(iCommandPStSp *self);
extern void iCommandPStSp_buildCommonInterface(iCommandPStSp *self, iCommand *common_ifc);

#endif




