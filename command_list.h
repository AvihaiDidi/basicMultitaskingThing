/*

This code is for defining a 'class' that handles a list of tasks and performs them in multiple threads in parallal.
Note that in practice the list of tasks functions like a stack.

TODO: maybe make it function like a queue instead in the future

*/

#ifndef COMMANDLIST
#define COMMANDLIST

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <pthread.h>
#include <unistd.h>

char UTF8_BEEP = 7;
int BUFFER_SIZE = 10000;
int DEFAULT_THREAD_COUNT = 4;
int BITS_IN_BYTE = 8; // very important const

// EXTREMLY important function, it make funny beep
void BEEP();

typedef struct command_list {
	int len;  // actual length of the commands list
	int clen; // length of the space allocated for command string pointers, must always be >= len
	char** commands;
	pthread_mutex_t lock; // lock for modifing the state of the command list
	pthread_mutex_t all_taken; // lock for when all threads are occupied, and you have to wait for some to become avilable
	int thread_count;
	pthread_t* threads;
	unsigned int activet;
} coms;

// initiazlie a new command list struct, prints an error message and returns NULL if initialization failed for some reason
coms* initComs(int thread_count);

// allocates more memory for future commands to be added, this should be done when len==clen
static void doubleClen(coms* c);

// returns the length s, \0 and \n are BOTH considered valid endpoints for the string
// if neither is present it'll probably segfault
static int strLen(char* s);

// copies src and returns the starting adress, src isn't modified
static char* copyStr(char* src);

// waits for all active threads to finish executing and then frees all memory used by the command list
void killComs(coms* c);

// returns the index of an active thread, if all threads are inactive returns -1
int getActiveThreadId(coms* c);

// returns the index of an inactive thread, if no thread is avilabile returns -1
int getInactiveThreadId(coms* c);

// toggles the activity status of a thread, this should be called whenever a thread starts executing a task (by the function starting the thread) and whenever the thread finishes it's task (by the thread itself)
static void toggleActive(coms* c, int id);

// removes an item from the top of the list and return it
// note that the responsibility of freeing the memory used by the task now falls on the code that called this function and not on the command list itself
char* popTask(coms* c);

// Just prints all items currently in the list, in the order they were added (which is the opposite of the order that they will be executed, because the list functions like a stack)
void printComs(coms* c);

// adds a command to the top of the list
// DOES NOT free the memory used by the command
void addCommand(coms* c, char* command);

// adds a bunch of commands to the top of the list
// DOES NOT free the memory used by the commands
void addCommands(coms* c, char* commands, int len);

// gets an initialized coms pointer and a file path, adds all of the commands from path to c
void addCommandsFromFile(coms* c, char* path);

/*	the function that the worker threads will execute, contains POINTER CRIMES
	args is a pointer to an array of void pointers, that each point to the following:
	args[0]		points to the command list struct
	args[1]		points to the command to be executed
	args[2]		points to an integer specifing the ID of this worker thread
	args[3]		points to a char specifing what the worker should do with the command ( see worker_thread_functions.h for details about what types of avilable ) */
void* workerThreadFunc(void* args);

/*	this is the big important function
	while len > 0, pops tasks and performs them
	type is the way the commands should be handeled (see the documentation in worker_thread_functions.h for details)
	note that this function DOESN'T WAIT FOR ALL THREADS TO EXIT, IT JUST ASSIGNS ALL OF THE WORK TO THE AVILABLE THREADS AND THEN EXITS
	this one should only run once in one thread, trying to use this function in several threads on the same command list will result in undefined behavior (i.e, it'll probably segfault) */
void processList(coms* c, char type);

#endif
