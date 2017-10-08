#include <stdlib.h>
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