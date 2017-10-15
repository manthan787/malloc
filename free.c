#include "free.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include "block.h"
#include "freelist.h"

Block* get_buddy(Block *);

void my_free(void *ptr) {
  Block *b = (Block *) (ptr - sizeof(Block));
  printf("Freeing block\n");
  print_block(b);
  unmark_block(b);
  Block *buddy = get_buddy(b);
  while (buddy != NULL && buddy->status != USED) {
    printf("Buddy block\n");
    print_block(buddy);
    if(buddy->next != NULL && buddy->previous != NULL) {
      buddy->next->previous = buddy->previous;
      buddy->previous->next = buddy->next;
      blocks[buddy->level] = buddy->previous;
    } else if(buddy->next != NULL) {
      buddy->next->previous = NULL;
      blocks[buddy->level] = buddy->next;
    } else if(buddy->previous != NULL) {
      buddy->previous->next = NULL;
      blocks[buddy->level] = buddy->previous;
    } else {
      blocks[buddy->level] = NULL;
    }
    if(buddy < b) b = buddy;
    b->level += 1;
    buddy = get_buddy(b);
  }

  if(blocks[b->level]) {
    Block* f = blocks[b->level];
    f->previous = b;
    b->next = f;
  }
  blocks[b->level] = b;
}

Block *get_buddy(Block *header) {
  if(header->level == MAX_INDEX) return NULL;
  void *buddy = (void *)((uintptr_t) header ^ (uintptr_t)pow(2, header->level + MIN_ORDER));
  printf("Buddy at %p\n", buddy);
  Block *b = (Block *) buddy;
  if(b && b->level == header->level) return b;
  return NULL;
}