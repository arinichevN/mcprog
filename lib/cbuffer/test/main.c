#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../interface.h"

#define BUF_LENGTH 16

void printAndMove(char **buf, size_t *buf_length){
	snprintf(*buf, *buf_length, "hello ");
	if(cbuffer_movePointerToStrEnd(buf, buf_length) != RESULT_SUCCESS){
		fprintf(stderr, "failed\n");
		return;
	}
	fprintf(stdout, "success\n");
}

int main() {
	char buf[BUF_LENGTH];
	size_t buf_length = BUF_LENGTH;
	memset(buf, 0, sizeof(char) * BUF_LENGTH);
	char *cbuf = buf;
	printAndMove(&cbuf, &buf_length);
	printAndMove(&cbuf, &buf_length);
	printAndMove(&cbuf, &buf_length);
	printAndMove(&cbuf, &buf_length);
	printAndMove(&cbuf, &buf_length);
	printAndMove(&cbuf, &buf_length);
	printAndMove(&cbuf, &buf_length);
	printf("%s(strlen: %ld)\n", buf, strlen(buf));
	return EXIT_SUCCESS;
}


