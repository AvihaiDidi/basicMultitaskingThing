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
	testFunction();
	return 0;
}

void testFunction() {
	printf("========================\ttestFunction meant to demonstrate use of this library. It prints this file (hence the comments in the first and last lines).\n");
	coms* c = initComs(20);
	if (c == NULL)
		return;
	addCommandsFromFile(c, "main.c");
	printf("========================\taddCommands\tfinished\n");
	processList(c, 's');
	printf("========================\tprocessQueue\tfinished\n");
	killComs(c);
	printf("========================\tkillComs\tfinished\n");
	printf("========================\tNote that since killComs doesn't wait for thread execution to complete, some lines might still get printed afterwards. This is working as intended.\n")
}

// THIS IS THE LAST LINE OF main.c
