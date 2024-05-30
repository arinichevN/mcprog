//					implementation dependencies
#include "interface.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>


/*
* using static functions and static global variables 
* we hide implementation details from other modules
*/
static ServerPackage request_n;
static ServerPackage *request = &request_n;

static ServerPackage response_n;
static ServerPackage *response = &response_n;

static serveResult_t serveFunctionDummy(ServerPackage *request, ServerPackage *response){
	return SERVE_RESULT_SILENT;
}

static serveFunc_t serve_function = serveFunctionDummy;

static void free_DUMMY(){
	;
}

static void free_REAL(){
	serverPackage_free(request);
	serverPackage_free(response);
}

static void (*freeFunction)() = free_DUMMY;

result_t stdServer_begin(serveFunc_t serve_func, size_t request_char_count, size_t response_char_count){
	result_t r = serverPackage_begin(request, request_char_count);
	if(r != RESULT_SUCCESS){
		fprintf(stderr, "stdServer_begin: failed to allocate memory for request package\n");
		return RESULT_FAILURE;
	}
	r = serverPackage_begin(response, response_char_count);
	if(r != RESULT_SUCCESS){
		serverPackage_free(request);
		fprintf(stderr, "stdServer_begin: failed to allocate memory for response package\n");
		return RESULT_FAILURE;
	}
	serve_function = serve_func;
	freeFunction = free_REAL;
	return RESULT_SUCCESS;
}
static result_t readRequest(char *buf, size_t buf_size){
	while (1){
		int c = fgetc(stdin);
		if(c == EOF) {
			fprintf(stderr, "readRequest(): end of file\n");
			return RESULT_FAILURE;
		}
		if(buf_size <= 1) goto read_overflow;
		*buf++ = c;
		buf_size--;
		if(c == '\n') {
			break;
		}
	}
	*buf = '\0';
	return RESULT_SUCCESS;
	read_overflow:
	while (1){
		int c = fgetc(stdin);
		if(c == EOF) {
			break;
		}
		if(c == '\n') {
			break;
		}
	}
	fprintf(stderr, "readRequest(): buffer overflow\n");
	return RESULT_FAILURE;
}
static void serve(){
	serverPackage_clear(request);
	if(readRequest(serverPackage_getCharBuffer(request), serverPackage_getCharBufferSize(request)) != RESULT_SUCCESS){
		fprintf(stderr, "serve(): error while reading request\n");
		return;
	}
	serverPackage_clear(response);
	serveResult_t sr = serve_function(request, response);
	if(sr == SERVE_RESULT_RESPOND){
		fprintf(stdout, "%s\n", serverPackage_getCharBuffer(response));
	}
}


void stdServer_control(){
	struct pollfd pfds[1] = {
		{STDIN_FILENO, POLLIN, 0}
	};
	int ret = poll(pfds, 1, 0);
	if (ret == -1) {
		perror ("poll");
		return;
	}
	if (!ret) {
		//printf("poll: timeout\n");
		return;
	}
	if (pfds[0].revents & POLLIN){
		serve();
		return;
	}
}


void stdServer_free(){
	freeFunction();
}

//					parts of this module
#include "iserver.c"
