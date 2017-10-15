#Sample Makefile for Malloc
CC=gcc
CFLAGS=-g -O0 -fPIC

all: check

clean:
	rm -rf libmalloc.so test1 malloc thread-test test *.o

test: test1.c block.c freelist.c malloc.c free.c realloc.c calloc.c
	gcc -g -o $@ $^ -lm

thread-test: thread-test.c block.c freelist.c malloc.c calloc.c realloc.c free.c
	gcc -g -o $@ $^ -lm -lpthread

test-run: test
	./test

malloc: malloc.c freelist.c block.c
	gcc $(CFLAGS) -c $^

libmalloc.so: malloc.o freelist.o block.o free.o calloc.o realloc.o
	$(CC) -shared -o $@ $^ -lm

test1: test1.o
	$(CC) $(CFLAGS) $< -o $@

t-test1: t-test1.o
	$(CC) $(CFLAGS) $< -o $@ -lpthread

%.o: %.c
	$(CC) $(CFLAGS) $< -c -o $@ -lm -lpthread

check:	libmalloc.so t-test1
	LD_PRELOAD=`pwd`/libmalloc.so ./t-test1

dist: clean
	dir=`basename $$PWD`; cd ..; tar cvf $$dir.tar ./$$dir; gzip $$dir.tar