#include "free.h"
#include <stdlib.h>
#include <stdio.h>
#include "block.h"
#include "freelist.h"

Block* get_buddy(Block *);

void my_free(void *ptr) {
  Block *b = (Block *) (ptr - sizeof(Block));
  printf("Freeing block\n");
  print_block(b);
  unmark_block(b);
  Block *buddy = get_buddy(b);
  if(buddy == NULL) {
    blocks[b->level] = b;
  }
  while (buddy != NULL && buddy->status != USED) {
    printf("Buddy block\n");
    print_block(buddy);
    Block *merged_block;
    int level = buddy->level;
    if(buddy->startAddr > b->startAddr) { // Buddy is to the right
      merged_block = new_block(b->startAddr, buddy->level + 1);
      merged_block->next = buddy->next;
      merged_block->previous = b->previous;
      if(buddy->next != NULL) buddy->next->previous = merged_block;
    } else {
      merged_block = new_block(buddy->startAddr, buddy->level + 1);
      merged_block->next = b->next;
      merged_block->previous = buddy->previous;
    }

    // Blocks at level are merged, so make the free list for that NULL
    blocks[level] = NULL;
    if(blocks[level + 1] == NULL) {
      blocks[level + 1] = merged_block;
    } else {
      if(merged_block < blocks[level + 1]) {
        Block* tmp = blocks[level + 1];
        blocks[level + 1] = merged_block;
        tmp->previous = merged_block;
      } else {
        merged_block->previous = blocks[level + 1];
        merged_block->next = blocks[level + 1]->next;
        blocks[level + 1]->next = merged_block;
      }
    }
    buddy = get_buddy(merged_block);
  }
}

Block *get_buddy(Block *header) {
  if (header->next && header->next->level == header->level) {
    return header->next;
  } else if (header->previous && header->previous->level == header->level) {
    return header->previous;
  } else {
    return NULL;
  }
}