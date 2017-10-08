#include <stdlib.h>
#include <stdio.h>
#include "block.h"

Block* new_block(void* addr) {
	Block* b = (Block *) addr;
	b->startAddr = addr;
	b->size = 0;
	b->next = NULL;
	b->previous = NULL;
	b->status = FREE;
	return b;
}

void* alloc_block(Block *b, size_t totalSize) {
	Block *freeblock = b;
    freeblock->size = totalSize;
    freeblock->status = USED;
    b = freeblock->next;
    if(b != NULL) b->previous = NULL;
    freeblock->next = NULL;
    return freeblock->startAddr + sizeof(Block);
}

void print_block(Block* b, int level) {
	printf("----- Block Information (%d)-----\n", level);
	printf("\taddr: %p\n", b->startAddr);
	printf("\tsize: %zu\n", b->size);
	printf("\tstatus: %d\n", b->status);
	printf("\tNext: %p\n", b->next);
	printf("\tPrevious: %p\n", b->previous);
	printf("------------------------------\n");
}

void print_blocklist(Block **blocks, int length) {
	int i;
	for(i = 0; i < length; i++) {
		printf("Level %d\n", i);
		Block *b = blocks[i];
		while(b != NULL) {
			print_block(b, i);
			b = b->next;
		}
	}
}