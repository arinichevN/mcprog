#ifndef SERVER_H
#define SERVER_H

#include "../ServerPackage/interface.h"

typedef enum {
	SERVE_RESULT_UNKNOWN,
	SERVE_RESULT_SILENT,
	SERVE_RESULT_RESPOND
} serveResult_t;

typedef serveResult_t (*serveFunc_t)(ServerPackage *request, ServerPackage *response);

#endif




