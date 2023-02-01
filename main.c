// THIS IS THE FIRST LINE OF main.c
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#include "command_list.h"

void testFunction();
void testFunction2();

int main() {
	testFunction();
	return 0;
}

/* 	testFunction meant to demonstrate use of this library.
	It prints this file (hence the comments in the first and last lines) */
void testFunction() {
	coms* c = initComs(20);
	if (c == NULL)
		return;
	addCommandsFromFile(c, "main.c");
	printf("========================\taddCommands\tfinished\n");
	processList(c, 'p');
	printf("========================\tprocessList\tfinished\n");
	printf("========================\tNote that since processQueue doesn't wait for thread execution to complete, some lines might still get printed afterwards. This is working as intended.\n");
	killComs(c);
	printf("========================\tkillComs\tfinished\n");
}

/* 	testFunction meant to demonstrate use of this library.
	It prints this file (hence the comments in the first and last lines)
	unlike the first testFunction, it waits for all threads to finish executing before exiting */
void testFunction2() {
	coms* c = initComs(20);
	if (c == NULL)
		return;
	addCommandsFromFile(c, "main.c");
	printf("========================\taddCommands\tfinished\n");
	processList(c, 'p');
	printf("========================\tprocessList\tfinished\n");
	waitForFinish(c);
	printf("========================\twaitForFinish\tfinished\n");
	killComs(c);
	printf("========================\tkillComs\tfinished\n");
}

// THIS IS THE LAST LINE OF main.c
