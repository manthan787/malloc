#include "block.h"
#include <stdio.h>
#include <stdlib.h>

Block* new_block(void* addr, int level) {
  Block* b = (Block*) addr;
  b->next = NULL;
  b->previous = NULL;
  b->status = FREE;
  b->level = level;
  return b;
}

Block* mark_block(Block* b) {
  Block* freeblock = b;
  freeblock->status = USED;
  return freeblock;
}

void unmark_block(Block *b) {
  b->status = FREE;
  b->level = -1;
}

void print_block(Block* b) {
  if (b != NULL) {
    printf("----- Block Information ------\n");
    printf("\taddr: %p\n", b);
    printf("\tstatus: %d\n", b->status);
    printf("\tNext: %p\n", b->next);
    printf("\tPrevious: %p\n", b->previous);
    printf("\tLevel: %d\n", b->level);
    printf("------------------------------\n");
  }
}