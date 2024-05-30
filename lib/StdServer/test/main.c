#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include "../interface.h"
#include "../../ServerPackage/interface.h"
#include "../../iServer/interface.h"

static serveResult_t serveFunction(ServerPackage *request, ServerPackage *response){
	char *req_b = serverPackage_getCharBuffer(request);
	char *res_b = serverPackage_getCharBuffer(response);
	size_t req_sl = strlen(req_b);
	size_t res_sl = strlen(res_b);
	snprintf(res_b, serverPackage_getCharBufferSize(response), "request length: %ld response length: %ld request was: %s\n", req_sl, res_sl, serverPackage_getCharBuffer(request));
	return SERVE_RESULT_RESPOND;
}

void clearAll(int s){
	stdServer_free();
	exit(EXIT_SUCCESS);
}

void manageSignals(){
	signal(SIGINT, clearAll);
	signal(SIGTERM, clearAll);
	signal(SIGHUP, clearAll);
	signal(SIGPIPE, clearAll);
}

void testRaw(){
	size_t request_char_count = 128;
	size_t response_char_count = 128;
	
	result_t r = stdServer_begin(serveFunction, request_char_count, response_char_count);
	if(r != RESULT_SUCCESS){
		return;
	}
	
	while(1){
		stdServer_control();
	}
}

void testIntreface(){
	iServer server_n;
	iServer *server = &server_n;
	stdServer_buildServerInterface(server);
	result_t r = iServer_begin(server, serveFunction);
	if(r != RESULT_SUCCESS){
		return;
	}
	iServer_greeting(server);
	while(1){
		iServer_control(server);
	}
}


int main() {
	manageSignals();
	testIntreface();
	return (EXIT_FAILURE);
}


