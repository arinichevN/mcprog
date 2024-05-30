#ifndef APP_SERVER_H
#define APP_SERVER_H

//					interface dependencies
#include "../../lib/result_t/interface.h"
#include "../../lib/server/interface.h"

extern result_t appServer_begin(serveFunc_t serveFunction);
extern void appServer_control();
extern void appServer_free();

#endif




