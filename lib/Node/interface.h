#ifndef NODE_H
#define NODE_H

//					interface dependencies
#include <stddef.h>
#include "../result_t/interface.h"

#define NODE_NAME_LENGTH_MAX	12
#define NODE_NAME_SIZE			(sizeof (char) * NODE_NAME_LENGTH_MAX)

#define NODE_INITIALIZER	{.name="", .next=NULL, .prev=NULL, .parent=NULL, .first_child=NULL, .data=NULL}

/*
* do not access structure elements from other modules directly, 
* although they are declared public for simplification of memory allocation.
*/
typedef struct node_st Node;
struct node_st {
	char name[NODE_NAME_LENGTH_MAX];
	Node *next;//this element helps to loop through all nodes in tree one by one
	Node *prev;//this element helps to insert and delete nodes
	Node *parent;//this element makes tree building possible 
	Node *first_child;//this element speeds up search process (but requires more memory)
	void *data;
};

extern void node_init(Node *self);
extern void node_connectToParent(Node *self, Node *node);
extern void node_connectToFirstChild(Node *self, Node *node);
extern void node_connectToPrevious(Node *self, Node *node);
extern void node_connectToNext(Node *self, Node *node);
extern void *node_getData(Node *self);
extern void node_setData(Node *self, void *data);
extern void node_setName(Node *self, const char *v);
extern const char *node_getName(Node *self);
extern size_t node_getNameSize(Node *self);
extern Node *node_findByName(Node *self, const char *name);

#endif
