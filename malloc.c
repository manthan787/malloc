#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <math.h>

// FIXME: Fix the internal fragmentation problem pointed out by the professor

#define ERROR(e) \
  {              \
    perror(e);   \
    exit(-1);    \
  }

// Enum to assign a status to the memory blocks
typedef enum block_status_t { USED, FREE } BlockStatus;

const int MIN_ORDER = 3, MAX_ORDER = 12;

// Memory Block to store metdata about the allocated memory blocks
typedef struct block_t {
  size_t size;
  struct block_t *next;
  struct block_t *previous;
  BlockStatus status;
} Block;

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
void init() {
  // Increment the data segment size by page size
  if ((heap = sbrk(sysconf(_SC_PAGESIZE))) == (void *)-1) ERROR("sbrk");
  blocks[9] = (Block *)heap;
  blocks[9]->size = 4096;
  blocks[9]->next = NULL;
  blocks[9]->previous = NULL;
  blocks[9]->status = FREE;
}

void *my_malloc(size_t size) {
  // Initialize if heap_break doesn't point to anything
  if (!heap) init();

  // Heap space already allocated
  size_t totalSize = size + sizeof(Block);
  printf("Order: %d\n", find_order(totalSize));
  // Find the order for the requested size
  // printf("Size asked for is %zu\n", totalSize );
}

int find_order(size_t size) {
  printf("Finding order for %zu bytes\n", size);
  int order = MAX_ORDER;
  while(order >= MIN_ORDER) {
    if(size <= pow(2, order) && size > pow(2, order - 1)) {
      break;
    }
    order--;
  }
  // The order index in the blocks array starts from 0,
  // so we subtract the smallest order out of the actual
  // order so that we can search the free blocks from the
  // correct list
  return order - MIN_ORDER;
}

int main() {
  my_malloc(2048);
  return 0;
}