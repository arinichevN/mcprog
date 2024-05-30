//					implementation dependencies
#include "interface.h"
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stddef.h>
#include "../../lib/Node/interface.h"
#include "../../lib/server/interface.h"
#include "../../lib/ServerPackage/interface.h"
#include "../../lib/iCommand/common/interface.h"
#include "../../lib/iCommand/ACDP/interface.h"
#include "../../lib/iCommand/EP/interface.h"
#include "../../lib/iCommand/PStSp/interface.h"

#define NAME_DELIMITER			'/'
#define DATA_DELIMITER			' '
#define PACKAGE_END_DELIMITER	'\n'

#define NAME_LENGTH_MAX			16

#define SBUFFER_LENGTH_MAX		64

/*
* using static functions and static global variables 
* we hide implementation details from other modules
*/
static const size_t SBUFFER_SIZE = sizeof(char) * SBUFFER_LENGTH_MAX;
static char sbuffer[SBUFFER_LENGTH_MAX];

static Node *root_node = NULL;

static const size_t PROTOCOL_COMMAND_SIZE_MAX = 16;

//					parts of this module
#include "parse.c"
#include "icommand.c"

static serveResult_t serveFunction(ServerPackage *request, ServerPackage *response){
	char *response_buffer = serverPackage_getCharBuffer(response);
	size_t response_bsize = serverPackage_getCharBufferSize(response);
	
	char *request_buffer = serverPackage_getCharBuffer(request);
	size_t request_length = serverPackage_getCharBufferLength(request);
	
	Node *node = getNodeByPath(root_node, request_buffer, request_length);
	if(node == NULL) {
		snprintf(response_buffer, response_bsize, "node not found\n");
		return SERVE_RESULT_RESPOND;
	}
	return serveCommand(node_getData(node), request, response);
}

serveFunc_t protocol_getServeFunction(){
	return serveFunction;
}

void protocol_setRootNode(Node *node){
	root_node = node;
}

#undef SBUFFER_LENGTH_MAX
#undef NAME_LENGTH_MAX
#undef NAME_DELIMITER
#undef DATA_DELIMITER
#undef PACKAGE_END_DELIMITER
