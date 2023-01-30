/*

Function documentation (comments) are in the header file
This file just has the implementations

*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

#include "command_list.h"

void BEEP() {
	printf("%c", UTF8_BEEP);
}

coms* initComs(int thread_count) {
	if (sizeof(unsigned int) * BITS_IN_BYTE < thread_count) {
		printf("activet is represented using an unsigned int and can only keep track of %lu threads, %d is too high for it.\n", sizeof(unsigned int) * BITS_IN_BYTE, thread_count);
		return NULL;
	}
	coms* c = malloc(sizeof(coms));
	c->len = 0;
	c->clen = 1;
	if (pthread_mutex_init(&c->lock, NULL) != 0) {
		printf("mutex init failed, stoopid\n");
		free(c);
		return NULL;
	}
	c->commands = malloc(sizeof(char*) * c->clen);
	c->thread_count = thread_count;
	c->threads = malloc(sizeof(pthread_t)*c->thread_count);
	c->activet = 0;
	return c;
}

static void doubleClen(coms* c) {
	c->clen *= 2;
	char** temp = malloc(sizeof(char*) * c->clen);
	for (int i=0;i<c->len;i++)
		temp[i] = c->commands[i];
	free(c->commands);
	c->commands = temp;
}

static int strLen(char* s) {
	if (s[0] == '\0' || s[0] == '\n')
		return 0;
	char c;
	int i = 0;
	do {
		c = s[i++];
	} while (c != '\0' && s[0] != '\n');
	return i;
}

static char* copyStr(char* src) {
	char c;
	int i = 0;
	char* tar = malloc(sizeof(char)*(strLen(src) + 1));
	do {
		c = src[i];
		if (c == '\n') {
			tar[i] == '\0';
			break;
		}
		tar[i++] = c;
	} while (c != '\0' && c != '\n');
	return tar;
}

void killComs(coms* c) {
	for (int i=0;i<c->thread_count;i++)
		pthread_join(c->threads[i], NULL);
	free(c->threads);
	for (int i=0;i<c->len;i++)
		free(c->commands[i]);
	free(c->commands);
	pthread_mutex_destroy(&c->lock);
	free(c);
}

int getActiveThreadId(coms* c) {
	int mask = 1;
	pthread_mutex_lock(&c->lock);
	for (int i=0;i<c->thread_count;i++) {
		if (mask & c->activet)
			return i;
		mask = mask << 1;
	}
	pthread_mutex_unlock(&c->lock);
	return -1;
}

int getInactiveThreadId(coms* c) {
	int mask = 1;
	pthread_mutex_lock(&c->lock);
	for (int i=0;i<c->thread_count;i++) {
		if (mask & ~c->activet) {
			pthread_mutex_unlock(&c->lock);
			return i;
		}
		mask = mask << 1;
	}
	pthread_mutex_unlock(&c->lock);
	return -1;
}

static void toggleActive(coms* c, int id) {
	pthread_mutex_lock(&c->lock);
	if (c->thread_count <= id || id < 0) {
		printf("%d\t is bad thread id\n", id);
		pthread_mutex_unlock(&c->lock);
		return;
	}
	c->activet = c->activet ^ (1 << id);
	pthread_mutex_unlock(&c->lock);
}

char* popTask(coms* c) {
	pthread_mutex_lock(&c->lock);
	if (c->len == 0) {
		pthread_mutex_unlock(&c->lock);
		return NULL;
	}
	char* task = c->commands[c->len - 1];
	c->len--;
	pthread_mutex_unlock(&c->lock);
	return task;
}

void printComs(coms* c) {
	pthread_mutex_lock(&c->lock);
	for (int i=0;i<c->len;i++)
		printf("%d\t%s\n", i+1, c->commands[i]);
	pthread_mutex_unlock(&c->lock);
}

void addCommand(coms* c, char* command){
	printf("IMPLEMENT THIS\n"); // TODO: IMPLEMENT THIS, AND USE IT IN addCommandsFromFile()
}

void addCommandsFromFile(coms* c, char* path) {
	FILE* f;
	char* buffer = malloc(BUFFER_SIZE * sizeof(char));
	f = fopen(path, "r");
	if (f == NULL) {
		printf("Failed to open the file '%s'.\n", path);
		free(buffer);
	}
	pthread_mutex_lock(&c->lock);
	while (fgets(buffer, BUFFER_SIZE, f) != NULL) {
		c->commands[c->len] = copyStr(buffer);
		c->len++;
		if (c->len == c->clen)
			doubleClen(c);
	}
	pthread_mutex_unlock(&c->lock);
	free(buffer);
	fclose(f);
}

void* workerThreadFunc(void* args) {
	// pointer crimes, extremly illegal
	coms* c = (coms*)(((void**)args)[0]);
	char* task = (char*)(((void**)args)[1]);
	int* worker_id = (int*)(((void**)args)[2]);
	// TODO: you need to get the command type from here (char*)(((void**)args)[3])
	int id_temp = *worker_id;
	//TODO: actually perform a task, for now just print it and sleep
	// Do task
	printf("%s\n", task); // TODO: implement all the possible actions in worker_thread_functions
	sleep(1);
	// set self to inactive and quit
	toggleActive(c, id_temp);
	free(worker_id);
	free(task);
	free(args);
}

void processList(coms* c) {
	while (0 < c->len) {
		char* task = popTask(c);
		int* worker_id = malloc(sizeof(int));
		*worker_id = getInactiveThreadId(c);
		int time_counter = 1;
		while (*worker_id == -1) {
			sleep(1); //TODO: this is dumb, use a second mutex instead of this crap
			*worker_id = getInactiveThreadId(c);
		}
		toggleActive(c, *worker_id);
		void** args = malloc(sizeof(void*) * 3);
		args[0] = c;
		args[1] = task;
		args[2] = worker_id;
		// TODO: add the task type to the 3rd (4th) element here
		pthread_create(&c->threads[*worker_id], NULL, workerThreadFunc, (void*)args);
	}
}
