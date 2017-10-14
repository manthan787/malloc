#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <math.h>
#include <errno.h>
#include "block.h"
#include "freelist.h"
#include "config.h"

// FIXME: Fix the internal fragmentation problem pointed out by the professor

#define ERROR(e) \
  {              \
    perror(e);   \
    exit(-1);    \
  }

// Location of break marker in the heap
static void *heap = NULL;

/**
 * Initialize the malloc library by extending heap size
 */
void init_heap() {
  // Increment the data segment size by page size
  if ((heap = sbrk(sysconf(_SC_PAGESIZE))) == (void *)-1) ERROR("sbrk");
  printf("block starts at %p address\n", heap);
  blocks[MAX_INDEX] = new_block(heap, MAX_INDEX);
}

void *my_malloc(size_t size) {
  // Initialize if heap_break doesn't point to anything
  if (!heap) init_heap();

  // Heap space already allocated
  size_t totalSize = size + sizeof(Block);

  // Find the order of the given size
  int level = find_level(totalSize);
  printf("Level is %d for request %zu\n", level, size);
  partition_blocks(blocks, level);

  if(blocks[level] != NULL) {
    Block* allocated = mark_block(blocks[level], totalSize);
    blocks[level] = (Block *)allocated->next;
    if(blocks[level]) blocks[level]->previous = allocated->previous;
    allocated->next = NULL;
    allocated->previous = NULL;
    printf("Allocated block \n");
    print_block(allocated);
    return allocated->startAddr + sizeof(Block);
  } else if(level == MAX_INDEX) {
    printf("Need a sbrk call\n");
  } else {
    errno = ENOMEM;
    return NULL;
  }
}

int find_level(size_t size) {
  // FIXME: Handle sizes bigger than 2**MAX_ORDER
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