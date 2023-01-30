// random crap, not a header file but whatever
// also not actually random, this is just the work queue functions

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

char UTF8_BEEP = 7;
int BUFFER_SIZE = 10000;
int DEFAULT_THREAD_COUNT = 4;
int BITS_IN_BYTE = 8; //bery important const

void BEEP() {
	printf("%c", UTF8_BEEP);
}

//TODO: make sure you use mutex around all non-static function, but not in any static function
//TODO: once that's done, make sure that all calls to static functions are in locked regions
//TODO: idk just don't try to lock it twice in a row? maybe add a prefix to funcs that lock it?

typedef struct command_list {
	int len;  // actual length of the commands list
	int clen; // length of the space allocated for commands, must always be >= len
	char** commands;
	pthread_mutex_t lock;
	int thread_count;
	pthread_t* threads;
	unsigned int activet;
} coms;

coms* initComs(int thread_count);

coms* initComsDefault();

// allocates more memory for future commands to be added, this should be done when len==clen
static void doubleClen(coms* c);

static int strLen(char* s);

static char* copyStr(char* src);
int getActiveThreadId(coms* c);

void killComs(coms* c);

// returns the index of an active thread, if all threads are inactive returns -1
int getActiveThreadId(coms* c);

// returns the index of an inactive thread, if no thread is avilabile returns -1
int getInactiveThreadId(coms* c);

void toggleActive(coms* c, int id);

char* popTask(coms* c);

void printComs(coms* c);
// gets an initialized coms pointer and a file path, adds all of the commands from path to c
void addCommands(coms* c, char* path);
void* workerThreadFunc(void* args);
// this is the big important function
// while len > 0, remove tasks and perform them
// note that this function DOESN'T WAIT FOR ALL THREADS TO EXIT, IT JUST ASSIGNS ALL OF THE WORK TO THE AVILABLE THREADS AND THEN EXITS
// this one should only run once in one thread
void processQueue(coms* c);
