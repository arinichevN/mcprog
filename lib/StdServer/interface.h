#ifndef STD_SERVER_H
#define STD_SERVER_H

//					interface dependencies
#include <stddef.h>
#include "../result_t/interface.h"
#include "../server/interface.h"
#include "../ServerPackage/interface.h"
#include "../iServer/interface.h"

extern result_t stdServer_begin(serveFunc_t serve_func, size_t request_char_count, size_t response_char_count);
extern void stdServer_control();
extern void stdServer_free();
extern void stdServer_buildServerInterface(iServer *ifc);

#endif




