#ifndef STDIO_SERVER_H
#define STDIO_SERVER_H

//					interface dependencies
#include <stddef.h>
#include "../result_t/interface.h"
#include "../server/interface.h"
#include "../ServerPackage/interface.h"
#include "../iServer/interface.h"

extern result_t tcpServer_begin(serveFunc_t serve_func, size_t request_char_count, size_t response_char_count, int port);
extern void tcpServer_control();
extern void tcpServer_free();
extern void tcpServer_buildServerInterface(iServer *ifc);

#endif




