//					implementation dependencies
#include "interface.h"
#include <stdio.h>
#include <stdlib.h>


//this is implementation of Item type. Do like this if you want
//hide elements of this structure from client modules, but in this case 
//you will have some problems in memory allocation because of unknown size of
//Item for client modules (see comments in interface.h file)
/*	struct item_st{
*		elements...
*	};
*/

/*
* using static functions and static global variables 
* we hide implementation details from other modules
*/
static void start_DUMMY(Item *self);
static void start_REAL(Item *self);

static void stop_DUMMY(Item *self);
static void stop_REAL(Item *self);

static void control_IDLE(Item *self);
static void control_STARTING(Item *self);
static void control_STOPPING(Item *self);
static void control_STEP1(Item *self);
static void control_STEP2(Item *self);
static void control_STEP_REP(Item *self);

static void onStartCommand(void *vself);
static void onStopCommand(void *vself);
static void onPrintCommand(void *vself, char *buf, size_t buf_size);

//					parts of this module
#include "icommand.c"

size_t item_getSize(){
	return sizeof(Item);
}

Item *item_new(){
	Item *out = malloc(sizeof (Item));
	if(out == NULL){
#ifdef MODE_TEST
		fprintf(stderr, "item_new(): failed to allocate dynamic memory\n");
#endif
	}
	return out;
}

void item_free(Item *self){
	free(self);
}

result_t item_begin(Item *self){
	self->id = ITEM_DEFAULT_ID;
	self->start_duration = sToStime(ITEM_DEFAULT_START_DURATION_S);
	self->stop_duration = sToStime(ITEM_DEFAULT_STOP_DURATION_S);
	self->start = start_REAL;
	self->stop = stop_DUMMY;
	self->control = control_IDLE;
	self->next = NULL;
	self->previous = NULL;
	iCommandPStSp *ic_pstsp = &self->icommand_pstsp;
	iCommand *ic = &self->icommand;
	iCommandPStSp_setParam(ic_pstsp, self, onPrintCommand, onStartCommand, onStopCommand);
	iCommandPStSp_buildCommonInterface(ic_pstsp, ic);
	node_init(&self->node);
	node_setData(&self->node, ic);
	return RESULT_SUCCESS;
}

void item_start(Item *self){
	self->start(self);
}

void item_stop(Item *self){
	self->stop(self);
}

void item_control(Item *self){
	self->control(self);
}


//===============================START==================================
static void start_DUMMY(Item *self){
	;
}

static void start_REAL(Item *self){
	self->start = start_DUMMY;
	ton_setInterval(&self->timer, self->start_duration);
	ton_reset(&self->timer);
	self->control = control_STARTING;
	self->stop = stop_REAL;
}


//===============================STOP===================================
static void stop_DUMMY(Item *self){
	;
}

static void stop_REAL(Item *self){
	self->stop = stop_DUMMY;
	ton_setInterval(&self->timer, self->stop_duration);
	ton_reset(&self->timer);
	self->control = control_STOPPING;
	self->start = start_REAL;
}


//================================RUN===================================

static void control_IDLE(Item *self){
	;
}

static void control_STARTING(Item *self){
	if(ton(&self->timer)){
		self->control = control_STEP1;
	}
}

static void control_STOPPING(Item *self){
	if(ton(&self->timer)){
		self->control = control_IDLE;
	}
}

static void control_STEP1(Item *self){
	self->control = control_STEP2;
}

static void control_STEP2(Item *self){
	self->control = control_STEP_REP;
}

static void control_STEP_REP(Item *self){
	self->control = control_STEP1;
}

Node *item_getNode(Item *self){
	return &self->node;
}

void item_connectToPrevious(Item *self, Item *previous){
	if(self == NULL) return;
	self->previous = previous;
	Node *previous_node = NULL;
	if(previous != NULL){
		previous_node = item_getNode(previous);
	}
	Node *self_node = &self->node;
	node_connectToPrevious(self_node, previous_node);
}

void item_connectToNext(Item *self, Item *next){
	if(self == NULL) return;
	self->next = next;
	Node *next_node = NULL;
	if(next != NULL){
		next_node = item_getNode(next);
	}
	Node *self_node = &self->node;
	node_connectToNext(self_node, next_node);
}

void item_setId(Item *self, int id){
	self->id = id;
}

int item_getId(Item *self){
	return self->id;
}

void item_setName(Item *self, const char *name){
	node_setName(&self->node, name);
}

const char *item_getName(Item *self){
	return node_getName(&self->node);
}

size_t item_getNameSize(Item *self){
	return node_getNameSize(&self->node);
}

Item *item_getNext(Item *self){
	return self->next;
}

Item *item_getPrevious(Item *self){
	return self->previous;
}

const char *item_getStateStr(Item *self){
	if(self->control == control_IDLE) return "IDLE";
	if(self->control == control_STARTING) return "STARTING";
	if(self->control == control_STOPPING) return "STOPPING";
	if(self->control == control_STEP1) return "RUN";
	if(self->control == control_STEP2) return "RUN";
	if(self->control == control_STEP_REP) return "RUN";
	return "?";
}

void item_print(Item *self, char *buf, size_t buf_size){
	snprintf(buf, buf_size, "Item: name:%s state:%s", item_getName(self), item_getStateStr(self));
}

