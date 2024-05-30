//					implementation dependencies
#include "interface.h"
#include <stddef.h>
#include <string.h>

void node_init(Node *self){
	memset(self->name, 0, NODE_NAME_SIZE);
	self->next = NULL;
	self->prev = NULL;
	self->parent = NULL; 
	self->first_child = NULL;
	self->data = NULL;
}

void node_makeRoot(Node *self){
	self->prev = NULL;
	self->parent = NULL;
}

void node_connectToParent(Node *self, Node *node){
	if(self == NULL) return;
	self->parent = node;
}

void node_connectToFirstChild(Node *self, Node *node){
	if(self == NULL) return;
	self->first_child = node;
}

void node_connectToPrevious(Node *self, Node *node){
	if(self == NULL) return;
	self->prev = node;
}

void node_connectToNext(Node *self, Node *node){
	if(self == NULL) return;
	self->next = node;
}

void node_deleteChildless(Node *self){
	self->prev->next = self->next;
	self->next->prev = self->prev;
	if(self->parent->first_child == self && self->parent == self->next->parent){
		self->parent->first_child = self->next;
	}
}

void *node_getData(Node *self){
	return self->data;
}

void node_setData(Node *self, void *data){
	self->data = data;
}

void node_setName(Node *self, const char *v){
	memcpy(self->name, v, NODE_NAME_SIZE);
}
const char *node_getName(Node *self){
	return self->name;
}
size_t node_getNameSize(Node *self){
	return NODE_NAME_SIZE;
}
Node *node_findByName(Node *self, const char *name){
	if(self == NULL) return NULL;
	if(strncmp(self->name, name, NODE_NAME_LENGTH_MAX) == 0){
		return self;
	}
	return node_findByName(self->next, name);
}
