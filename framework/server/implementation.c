//					implementation dependencies
#include "interface.h"
#include <stdlib.h>
#include <stdio.h>
#include "../../lib/result_t/interface.h"
#include "../../lib/iServer/interface.h"
#include "../../lib/StdServer/interface.h"
#include "../../lib/TcpServer/interface.h"
#include "../../lib/server/interface.h"

/*
* using static functions and static global variables 
* we hide implementation details from other modules
*/
static iServer server_n;
static iServer *server = &server_n;

static void freeDummy(){;}
static void freeReal(){iServer_free(server);}
static void (*freeFunc)() = freeDummy;

static void controlDummy(){;}
static void controlReal(){iServer_control(server);}
static void (*controlFunc)() = controlDummy;

static char getFirstChar(){
	char fc = 0;
	for(size_t i=0; ;i++){
		int s = fgetc(stdin);
		if(s==EOF){
			perror("getFirstChar()");
			continue;
		}
		if(s=='\n'){
			break;
		}
		if(i == 0){
			fc = s;
			continue;
		}
	}
	return fc;
}

static void selectServer(){
	printf("%32s", "Select server you want to use:\n");
	printf("%32s", "Standart I/O Server (s)\n");
	printf("%32s", "or\n");
	printf("%32s", "TCP Server (t)\n");
	while(1){
		printf("(s|t)?");
		char s = getFirstChar();
		if(s == 's'){
			stdServer_buildServerInterface(server);
			break;
		} else if(s == 't'){
			tcpServer_buildServerInterface(server);
			break;
		} else {
			continue;
		}
	}
}

result_t appServer_begin(serveFunc_t serveFunction) {
	iServer_init(server);
	freeFunc = freeReal;
	controlFunc = controlReal;
	selectServer();
	if(iServer_begin(server, serveFunction) != RESULT_SUCCESS){
		return RESULT_FAILURE;
	}
	iServer_greeting(server);
	return RESULT_SUCCESS;
}

void appServer_control(){
	controlFunc();
}

void appServer_free(){
	freeFunc();
}

