// THIS IS THE FIRST LINE OF main.c
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#include "command_list.h"

void uhhh1();
void uhhh2();
void uhhh3();

int main() {
	uhhh3();
	return 0;
}

// same as uhhh2 but to miscfuncs.h and with MORE threads, just to see what happens
void uhhh3() {
	printf("uh\n");
	coms* c = initComs(32);
	if (c == NULL)
		return;
	addCommandsFromFile(c, "main.c");
	printf("========================\taddCommands\tfinished\n");
	processList(c, 's');
	printf("========================\tprocessQueue\tfinished\n");
	killComs(c);
	printf("========================\tkillComs\tfinished\n");
}

// the sorta real thing, make a queue and execute (print&wait) it in parallal
void uhhh2() {
	coms* c = initComs(20);
	if (c == NULL)
		return;
	addCommandsFromFile(c, "main.c");
	processList(c, 's');
	printf("processQueue\tfinished\n");
	killComs(c);
	printf("killComs\tfinished\n");
}

// basic func to test that the queue works
void uhhh1() {
	coms* c = initComs(4);
	addCommandsFromFile(c, "main.c");
	printComs(c);
	char* task = popCommand(c);
	while (task != NULL) {
		printf("%d\t%s\n", c->len + 1, task);
		free(task);
		task = popCommand(c);
	}
	killComs(c);
}
// THIS IS THE LAST LINE OF main.c
