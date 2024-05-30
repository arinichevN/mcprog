#ifndef PROTOCOL_H
#define PROTOCOL_H

//					interface dependencies
#include "../../lib/server/interface.h"
#include "../../lib/Node/interface.h"

extern serveFunc_t protocol_getServeFunction();
extern void protocol_setRootNode(Node *node);

#endif




