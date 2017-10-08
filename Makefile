#Sample Makefile for Malloc
CC=gcc
CFLAGS=-g -O0 -fPIC

all: check

clean:
	rm -rf libmalloc.so malloc.o test1 malloc malloc.o block.o

test: test1.c block.c freelist.c malloc.c
	gcc -g -o $@ $^ -lm
	./test

malloc.o:
	gcc -g -o malloc.o malloc.c -lm

libmalloc.so: malloc.o
	$(CC) -shared -o libmalloc.so malloc.o

test1: test1.o
	$(CC) $(CFLAGS) $< -o $@

# For every XYZ.c file, generate XYZ.o.
%.o: %.c
	$(CC) $(CFLAGS) $< -c -o $@

check:	libmalloc.so test1
	LD_PRELOAD=`pwd`/libmalloc.so ./test1

dist: clean
	dir=`basename $$PWD`; cd ..; tar cvf $$dir.tar ./$$dir; gzip $$dir.tar