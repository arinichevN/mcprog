#ifndef APP_NODE_H
#define APP_NODE_H

//					interface dependencies
#include <stddef.h>
#include "../../lib/Node/interface.h"

extern void appNode_begin(const char *name, void (*onExitCommand)(), void (*onPrintCommand)(void *, char *, size_t));
extern Node *appNode_getRootNode();
extern void appNode_connectToFirstChildNode(Node *first_child);

#endif




