//					implementation dependencies
#include "interface.h"
#include <stdlib.h>
#include <stdio.h>
#include "../../lib/iCommand/common/interface.h"
#include "../../lib/iCommand/EP/interface.h"

/*
* using static functions and static global variables 
* we hide implementation details from other modules
*/
static Node node_n = NODE_INITIALIZER;
static Node *node = &node_n;

static iCommandEP icommand_ep;
static iCommand icommand = ICOMMAND_INITIALIZER;

void appNode_begin(const char *name, void (*onExitCommand)(), void (*onPrintCommand)(void *, char *, size_t)){
	iCommandEP *ic_ep = &icommand_ep;
	iCommand *ic = &icommand;
	iCommandEP_setParam(ic_ep, NULL, onExitCommand, onPrintCommand);
	iCommandEP_buildCommonInterface(ic_ep, ic);
	node_setData(node, ic);
	node_setName(node, name);
}

Node *appNode_getRootNode(){
	return node;
}

void appNode_connectToFirstChildNode(Node *first_child){
	node_connectToFirstChild(node, first_child);
	node_connectToNext(node, first_child);
	node_connectToParent(first_child, node);
	node_connectToPrevious(first_child, node);
}
