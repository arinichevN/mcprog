//					implementation dependencies
#include "interface.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
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

static int tcp_fd = -1;

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
	close(tcp_fd);
}

static void (*freeFunction)() = free_DUMMY;

static result_t tcpBegin (int *fd, int port) {
	struct sockaddr_in addr;
	if(( *fd = socket(PF_INET, SOCK_STREAM, 0)) == -1){
		perror("tcpBegin(): socket");
		return RESULT_FAILURE;
	}
	memset(( char *)&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	int flag = 1;
	if(setsockopt(*fd, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(int)) < 0){
		close(*fd);
		perror("tcpBegin(): setsockopt()");
		return RESULT_FAILURE;
	}
	if(bind(*fd, (struct sockaddr* )(&addr), sizeof(addr)) == -1){
		close(*fd);
		perror("tcpBegin(): bind()");
		return RESULT_FAILURE;
	}
	if(listen(*fd, 7)!= 0){
		close(*fd);
		perror("tcpBegin(): listen()");
		return RESULT_FAILURE;
	}
	return RESULT_SUCCESS;
}

result_t tcpServer_begin(serveFunc_t serve_func, size_t request_char_count, size_t response_char_count, int port){
	result_t r = serverPackage_begin(request, request_char_count);
	if(r != RESULT_SUCCESS){
		fprintf(stderr, "tcpServer_begin: failed to allocate memory for request package\n");
		return RESULT_FAILURE;
	}
	r = serverPackage_begin(response, response_char_count);
	if(r != RESULT_SUCCESS){
		serverPackage_free(request);
		fprintf(stderr, "tcpServer_begin: failed to allocate memory for response package\n");
		return RESULT_FAILURE;
	}
	r = tcpBegin(&tcp_fd, port);
	if(r != RESULT_SUCCESS){
		serverPackage_free(request);
		serverPackage_free(response);
		fprintf(stderr, "tcpServer_begin: failed to start tcp server\n");
		return RESULT_FAILURE;
	}
	serve_function = serve_func;
	freeFunction = free_REAL;
	return RESULT_SUCCESS;
}

static result_t readRequest(int fd, char *buf, size_t buf_size){
	while (1){
		char c = 0;
		ssize_t n = read(fd, &c, 1);
		if(n == -1){
			perror("readRequest()");
			return RESULT_FAILURE;
		}
		if(n == 0) {
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
		char c = 0;
		ssize_t n = read(fd, &c, 1);
		if(n == -1){
			perror("readRequest()");
			return RESULT_FAILURE;
		}
		if(n == 0) {
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
	int conn_fd = accept(tcp_fd, (struct sockaddr*)NULL ,NULL);
	serverPackage_clear(request);
	if(readRequest(conn_fd, serverPackage_getCharBuffer(request), serverPackage_getCharBufferSize(request)) != RESULT_SUCCESS){
		fprintf(stderr, "serve(): error while reading request\n");
		close(conn_fd);
		return;
	}
	serverPackage_clear(response);
	serveResult_t sr = serve_function(request, response);
	if(sr == SERVE_RESULT_RESPOND){
		write(conn_fd, serverPackage_getCharBuffer(response), serverPackage_getCharBufferSize(response)); 
	}
	close(conn_fd);
}


void tcpServer_control(){
	struct pollfd pfds[1] = {
		{tcp_fd, POLLIN, 0}
	};
	int ret = poll(pfds, 1, 0);
	if (ret == -1) {
		perror ("poll");
		return;
	}
	if (!ret) {
		//printf ("poll: timeout\n");
		return;
	}
	if (pfds[0].revents & POLLIN){
		serve();
		return;
	}
}


void tcpServer_free(){
	freeFunction();
}

//					parts of this module
#include "iserver.c"
