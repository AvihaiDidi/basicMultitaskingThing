// THIS IS THE FIRST LINE OF main.c
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#include "miscfuncs.h"

void uhhh1();
void uhhh2();
void uhhh3();

int main() {
	uhhh3();
	return 0;
}

// same as uhhh2 but to miscfuncs.h and with MORE threads, just to see what happens
void uhhh3() {
	coms* c = initComs(32);
	if (c == NULL)
		return;
	addCommands(c, "miscfuncs.h"); addCommands(c, "miscfuncs.h"); addCommands(c, "miscfuncs.h");
	printf("========================\taddCommands\tfinished\n");
	processQueue(c);
	printf("========================\tprocessQueue\tfinished\n");
	killComs(c);
	printf("========================\tkillComs\tfinished\n");
}

// the sorta real thing, make a queue and execute (print&wait) it in parallal
void uhhh2() {
	coms* c = initComs(20);
	if (c == NULL)
		return;
	addCommands(c, "main.c");
	processQueue(c);
	printf("processQueue\tfinished\n");
	killComs(c);
	printf("killComs\tfinished\n");
}

// basic func to test that the queue works
void uhhh1() {
	coms* c = initComsDefault();
	addCommands(c, "main.c");
	printComs(c);
	char* task = popTask(c);
	while (task != NULL) {
		printf("%d\t%s\n", c->len + 1, task);
		free(task);
		task = popTask(c);
	}
	killComs(c);
}
// THIS IS THE LAST LINE OF main.c
