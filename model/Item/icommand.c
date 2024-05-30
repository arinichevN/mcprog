/*
* using static functions and static global variables 
* we hide implementation details from other modules
*/
static void onStartCommand(void *vself){
	Item *self = vself;
	item_start(self);
}

static void onStopCommand(void *vself){
	Item *self = vself;
	item_stop(self);
}

static void onPrintCommand(void *vself, char *buf, size_t buf_size){
	Item *self = vself;
	snprintf(buf, buf_size, "Item: name:%s state:%s\n", item_getName(self), item_getStateStr(self));
}
