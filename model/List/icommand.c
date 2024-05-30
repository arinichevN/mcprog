/*
* using static functions and static global variables 
* we hide implementation details from other modules
*/
static void onAddCommand(void *vself, const char *name){
	List *self = vself;
	if(name == NULL) {
		fprintf(stderr, "List: onAddCommand(): empty name is bad name\n");
		return;
	}
	Item *e_item = getItemByName(self, name);
	if(e_item != NULL){
		fprintf(stderr, "List: onAddCommand(): item exists, name should be unique\n");
		return;
	}
	Item *new_item = buffer_occupyItem(&self->buffer);
	if(new_item == NULL){
		fprintf(stderr, "List: onAddCommand(): failed to create new item\n");
		return;
	}
	if(item_begin(new_item) != RESULT_SUCCESS){
		buffer_freeItem(&self->buffer, new_item);
		fprintf(stderr, "List: onAddCommand(): failed to begin new item\n");
		return;
	}
	item_setName(new_item, name);
	Node *item_node = item_getNode(new_item);
	Node *self_node = &self->node;
	if(self->last == NULL){
		self->first = new_item;
		node_connectToFirstChild(self_node, item_node);
		node_connectToNext(self_node, item_node);
		node_connectToParent(item_node, self_node);
		node_connectToPrevious(item_node, self_node); 
	} else {
		item_connectToNext(self->last, new_item);
		item_connectToPrevious(new_item, self->last);
		node_connectToParent(item_node, self_node);
	}
	self->last = new_item;
}

static void onClearCommand(void *vself){
	List *self = vself;
	Item *item = self->first;
	while(item != NULL){
		Item *next = item_getNext(item);
		buffer_freeItem(&self->buffer, item);
		item = next;
	}
	Node *self_node = &self->node;
	node_connectToFirstChild(self_node, NULL);
	node_connectToNext(self_node, NULL);
	self->first = NULL;
	self->last = NULL;
}

static void onDeleteCommand(void *vself, const char *name){
	List *self = vself;
	Item *item = getItemByName(self, name);
	if(item != NULL){
		Item *previous_item = item_getPrevious(item);
		Item *next_item = item_getNext(item);
		item_connectToNext(previous_item, next_item);
		item_connectToPrevious(next_item, previous_item);
		if(self->first == item){
			self->first = next_item;
		}
		if(self->last == item){
			self->last = previous_item;
		}
		buffer_freeItem(&self->buffer, item);
		return;
	}
	fprintf(stderr, "List: onDeleteCommand(): item not found\n");
}

static void onPrintCommand(void *vself, char *buf, size_t buf_size){
	List *self = vself;
	snprintf(buf, buf_size, "list:\n");
	if(cbuffer_movePointerToStrEnd(&buf, &buf_size) != RESULT_SUCCESS){
		fprintf(stderr, "List: onPrintCommand(): buffer overflow(1)\n");
		return;
	}
	for(Item *item=self->first; item!=NULL; item=item_getNext(item)){
		snprintf(buf, buf_size, "\t");
		if(cbuffer_movePointerToStrEnd(&buf, &buf_size) != RESULT_SUCCESS){
			fprintf(stderr, "List: onPrintCommand(): buffer overflow(2)\n");
			return;
		}
		item_print(item, buf, buf_size);
		if(cbuffer_movePointerToStrEnd(&buf, &buf_size) != RESULT_SUCCESS){
			fprintf(stderr, "List: onPrintCommand(): buffer overflow(3)\n");
			return;
		}
		snprintf(buf, buf_size, "\n");
		if(cbuffer_movePointerToStrEnd(&buf, &buf_size) != RESULT_SUCCESS){
			fprintf(stderr, "List: onPrintCommand(): buffer overflow(2)\n");
			return;
		}
	}
}
