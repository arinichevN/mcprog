#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../interface.h"

#include <unistd.h>//for usleep()

#define N 10
#define NUMBER_OF_ITEMS 100

typedef void (*Command)(Item *);

Command fs[]  = {item_start, item_stop, item_control};

Command getRandomCommand(){
	int i = rand() % 3;
	return fs[i];
}

void testCommands(Item *item){
	srand(time(NULL));
	for(int i=0; i<NUMBER_OF_ITEMS; i++){
		Command f = getRandomCommand();
		f(item);
	}
}

void testRun(Item *item){
	for(int i=0; i<N; i++){
		item_control(item);
		usleep(100);
	}
}

void testAutomaticItem(){
	Item items[NUMBER_OF_ITEMS];
	for(int i=0; i<NUMBER_OF_ITEMS; i++){
		item_begin(&items[i]);
		item_setId(&items[i], i);
	}
	for(int i=0; i<NUMBER_OF_ITEMS; i++){
		Item *item = &items[i];
		item_start(item);
		testRun(item);
		item_stop(item);
	}
}

void testDynamicItems(){
	Item *items[NUMBER_OF_ITEMS];
	for(int i=0; i<NUMBER_OF_ITEMS; i++){
		Item *o = item_new();
		if(o==NULL){
			printf("failed to create new item at step %d\n", i);
			for(int j = 0; j<i; j++){
				Item *item = items[j];
				item_free(item);
			}
			return;
		}
		items[i] = o;
	}
	for(int i=0; i<NUMBER_OF_ITEMS; i++){
		Item *item = items[i];
		item_begin(item);
		item_setId(item, i);
	}
	for(int i=0; i<NUMBER_OF_ITEMS; i++){
		Item *item = items[i];
		item_start(item);
		testRun(item);
		item_stop(item);
	}
	for(int i=0; i<NUMBER_OF_ITEMS; i++){
		Item *item = items[i];
		testCommands(item);
	}
	for(int i=0; i<NUMBER_OF_ITEMS; i++){
		Item *item = items[i];
		item_free(item);
	}
	return;
}


int main(void) {
	printf("Test for items\n");
	testAutomaticItem();
	testDynamicItems();
	printf("Test done\n");
	return 0;
}
