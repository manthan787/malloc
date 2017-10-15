#include "free.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <assert.h>
#include "block.h"
#include "freelist.h"
#include "malloc.h"

Block* get_buddy(Block *);

/**
 * Free memory at given address `ptr`
 * This is done by combining buddies for the block to be freed until we either
 * comobine all of the free blocks or encounter a used memory block.
 * @param ptr address at which memory is to be freed
 */
void free(void *ptr) {
  if (ptr == NULL) return;
  pthread_mutex_lock(&mutex);
  stats.freereq += 1;
  Block *b = (Block *) ((char *)ptr - sizeof(Block));
  unmark_block(b);
  Block *buddy = get_buddy(b);
  while (buddy != NULL && buddy->status != USED) {
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
    Block* f = (Block *)blocks[b->level];
    f->previous = b;
    b->next = f;
  }
  blocks[b->level] = b;
  pthread_mutex_unlock(&mutex);
}

/**
 * Find a buddy block for the given block.
 *
 * To find a buddy we xor the address of the given block with its size
 * @param  header the block for which buddy is to be found
 * @return        returns the buddy block if exists, otherwise returns NULL
 */
Block *get_buddy(Block *header) {
  if(header->level == MAX_INDEX) return NULL;
  void *buddy = (void *)((uintptr_t) header ^ (uintptr_t)pow(2, header->level + MIN_ORDER));
  Block *b = (Block *) buddy;
  if(b && b->level == header->level) return b;
  return NULL;
}