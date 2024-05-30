//					implementation dependencies
#include "interface.h"
#include <stdlib.h>
#include <stdio.h>
#include "../../lib/stime_t/interface.h"
#include "../../lib/protocol/interface.h"
#include "../signals/interface.h"
#include "../server/interface.h"
#include "../node/interface.h"
#include "../list/interface.h"


/*
* using static functions and static global variables 
* we hide implementation details from other modules
*/
static void step_IDLE();
static void step_RUN();

static void (*control)() = step_IDLE;

static void clearAll(int s){
	appServer_free();
	appList_free();
	exit(EXIT_SUCCESS);
}

static void onExitCommand(){
	clearAll(0);
}

static void onPrintCommand(void *data, char *buf, size_t buf_size){
	snprintf(buf, buf_size, "this application is intended to show modular programming technics using C");
}

void app_begin() {
	appSignals_begin(clearAll);
	appNode_begin("app", onExitCommand, onPrintCommand);
	protocol_setRootNode(appNode_getRootNode());
	if(appServer_begin(protocol_getServeFunction()) != RESULT_SUCCESS){
		fprintf(stderr, "app_begin(): failed to start server\n");
		clearAll(0);
		return;
	}
	if(appList_begin() != RESULT_SUCCESS){
		fprintf(stderr, "app_begin(): failed to start list\n");
		clearAll(0);
		return;
	}
	appNode_connectToFirstChildNode(appList_getRootNode());
	control = step_RUN;
}

void app_control(){
	control();
}

static void step_IDLE(){
	stime_sleep(usToStime(1000));
}

static void step_RUN(){
	appServer_control();
	appList_control();
	stime_sleep(usToStime(1000));
}
