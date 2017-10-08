#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <math.h>
#include "block.h"

// FIXME: Fix the internal fragmentation problem pointed out by the professor

#define ERROR(e) \
  {              \
    perror(e);   \
    exit(-1);    \
  }

#define MAX_INDEX 9
#define MIN_INDEX 0

const int MIN_ORDER = 3, MAX_ORDER = 12;

// Location of break marker in the heap
static void *heap = NULL;

/**
 * Allowed memory blocks in this buddy allocation implementation
 * The biggest memory block allowed is 4096 bytes and the smallest
 * block allowed is 8 bytes.
 * blocks[0] stores blocks having size 8 bytes
 * and blocks[9] stores blocks having size 4096 bytes
 */
static Block *blocks[10];

/**
 * Initialize the malloc library by extending heap size
 */
void init_heap() {
  // Increment the data segment size by page size
  if ((heap = sbrk(sysconf(_SC_PAGESIZE))) == (void *)-1) ERROR("sbrk");
  printf("block starts at %p address\n", heap);
  blocks[9] = new_block(heap);
}

void partition_blocks(int order) {
  int split = MAX_INDEX + 1;
  printf("Start split at %d\n", split);
  while(blocks[order] == NULL && split > MIN_INDEX) {
    split--;
    if(blocks[split] == NULL) continue;
    int newsplit = split - 1;
    if(blocks[newsplit] == NULL) {
      printf("splitting %p into two\n", blocks[split]->startAddr);
      void *addr = blocks[split]->startAddr;
      blocks[split] = blocks[split]->next;
      Block *block1 = new_block(addr);
      Block *block2 = new_block(addr + (int)pow(2, newsplit + MIN_ORDER));
      block1->next = block2;
      block2->previous = block1;
      blocks[newsplit] = block1;
    }
  }
}

void *my_malloc(size_t size) {
  // Initialize if heap_break doesn't point to anything
  if (!heap) init_heap();

  // Heap space already allocated
  size_t totalSize = size + sizeof(Block);

  // Find the order of the given size
  int order = find_order(totalSize);

  if(blocks[order] != NULL) {
    Block* allocated = mark_block(blocks[order], totalSize);
    blocks[order] = (Block *)allocated->next;
    return allocated->startAddr + sizeof(Block);
  } else partition_blocks(order);

  // If there are no blocks available, break the existing blocks
  if(order == MAX_INDEX) {
    printf("Probably need to call sbrk now!\n");
    // FIXME: Abort or extend data segment using `sbrk`
  }
}

int find_order(size_t size) {
  printf("Finding order for %zu bytes\n", size);
  int order = MAX_ORDER;
  while(order >= MIN_ORDER) {
    if(size <= pow(2, order) && size > pow(2, order - 1)) {
      // The order index in the blocks array starts from 0,
      // so we subtract the smallest order out of the actual
      // order so that we can search the free blocks from the
      // correct list
      return order - MIN_ORDER;
    }
    order--;
  }
  return 0;
}

int main() {
  printf("Allocated memory at %p\n", my_malloc(900));
  printf("Allocated memory at %p\n", my_malloc(200));
  printf("Allocated memory at %p\n", my_malloc(900));
  printf("Allocated memory at %p\n", my_malloc(200));
  print_blocklist(blocks, 10);
  return 0;
}