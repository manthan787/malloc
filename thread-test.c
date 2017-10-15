/**
 * Test file for checking threads' requests for memory
 */

#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include "malloc.h"
#include "free.h"
#include "block.h"

void *func() {
	printf("Inside thread 1\n");
	void *p = malloc(50);
	void *p1 = realloc(p, 100);
	free(p);
	printf("-----Thread 1 Done----\n");
	return NULL;
}

void *func1() {
	printf("Inside thread 2\n");
	void *p = malloc(50);
	void *p1 = malloc(200);
	void *p2 = malloc(200);
	void *p3 = malloc(5000);
	void *p4 = realloc(p2, 300);
	free(p4);
	printf("-----Thread 2 Done----\n");
	return NULL;
}

int main() {
	pthread_t t1, t2, t3;
	pthread_create(&t1, NULL, func,  NULL);
	pthread_create(&t2, NULL, func1, NULL);
	pthread_create(&t3, NULL, func,  NULL);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_join(t3, NULL);
	malloc_stats();
	return 0;
}