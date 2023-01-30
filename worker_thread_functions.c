/*

Function documentation (comments) is in the header file
This file just has the implementations

*/

#include "worker_thread_functions.h"

void* commandHandler(char* command, char type) {
	switch (type) {
		case 'p':
			CHp(command);
			return NULL;
		break;
		case 's':
			CHs(command);
			return NULL;
		break;
		case 'b':
			return CHb(command);
		break;
		default:
			printf("Unsupported operation '%c', returning NULL\n", &type);
	}
}

static void CHp(char* command) {
	printf("%s\n", task);
}

static void CHs(char* command) {
	CHp(command);
	sleep(1);
}

static *void CHb(char* command) {
	printf("TODO: implement this\n"); // TODO: implement this
}
