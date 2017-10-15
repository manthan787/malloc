#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <math.h>
#include <errno.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include "malloc.h"
#include "freelist.h"
#include "config.h"

#define ERROR(e) \
  {              \
    perror(e);   \
    exit(-1);    \
  }

// Location of break marker in the heap

/**
 * Initialize the malloc library by extending heap size
 */
void init_heap() {
  // Increment the data segment size by page size
  if ((heap = sbrk(sysconf(_SC_PAGESIZE))) == (void *)-1) {
    errno = ENOMEM;
    ERROR("sbrk");
  }
  stats.numArenas += 1;
  blocks[MAX_INDEX] = new_block(heap, MAX_INDEX);
}

/**
 * Extends the heap, by extending heap size and adding more blocks
 * at the end of the freelist
 */
void extend_heap() {
  // Increment the data segment size by page size
  if ((heap = sbrk(sysconf(_SC_PAGESIZE))) == (void *)-1) {
    errno = ENOMEM;
    ERROR("sbrk");
  }
  // If free list for maximum index is not null, add the newly allocated
  // heapsize at the end of the list
  if(blocks[MAX_INDEX] != NULL) {
    Block* b = blocks[MAX_INDEX];
    while(b->next != NULL) {
      b = b->next;
    }
    b->next = new_block(heap, MAX_INDEX);
    b->next->previous = b;
    b->level = MAX_INDEX;
  }
  else blocks[MAX_INDEX] = new_block(heap, MAX_INDEX);
}


/**
 * Allocates memory for the given memory request of size `size`
 *
 * If this is the first request to this function, break pointer is
 * moved by size `PAGE SIZE`.
 * If there's a memory block available for requested `size`, it is
 * simply returned.
 * If there's no free block for requested `size`:
 *   - Check if there's a bigger block available. If so, the bigger block
 *     is broken into two until the request can be satisfied.
 *   - Otherwise the heap is extended again by PAGESIZE and the request is
 *     satisfied. The blocks are partitioned like in the previous step if
 *     required.
 *
 * If the memory request is bigger than the maximum limit for a free block
 * which is 4096 bytes, the request is satisfied using `mmap`. This memory
 * is represented in the `mallinfo` struct as `hblks`(blocks) and `hblkhd`(bytes)
 * @param  size [description]
 * @return      [description]
 */
void *malloc(size_t size) {
  pthread_mutex_lock(&mutex);
  stats.allocreq += 1;
  // Initialize if heap_break doesn't point to anything
  if (!heap) init_heap();
  size_t totalSize = size + sizeof(Block);
  // Find the order of the given size
  int level = find_level(totalSize);
  // Extend heap if the memory requested can not be satisfied
  if(level > MAX_INDEX) {
    int mem = (int) pow(2, level + MIN_ORDER);
    void *p = mmap(0, mem, PROT_READ | PROT_WRITE,
                   MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    assert(p != MAP_FAILED);
    Block* b = new_block(p, level);
    stats.hblks += 1;
    stats.hblkhd += totalSize;
    pthread_mutex_unlock(&mutex);
    return (char *) b + sizeof(Block);
  }
  // If there's not free block available on the free list for requested level
  // find the next big free block
  int free = level;
  while(blocks[free] != NULL && free < MAX_INDEX) {
    free++;
  }
  // If the biggest available index is MAX INDEX
  // then extend heap
  if(free == MAX_INDEX) {
    extend_heap();
  }
  // Partition blocks into smaller blocks if necessary
  partition_blocks(blocks, level);
  // Allocate Block
  if(blocks[level] != NULL) {
    Block* allocated = mark_block(blocks[level]);
    blocks[level] = (Block *) allocated->next;
    if(blocks[level]) blocks[level]->previous = allocated->previous;
    allocated->next = NULL;
    allocated->previous = NULL;
    stats.uordblks += totalSize;
    pthread_mutex_unlock(&mutex);
    return (char*) allocated + sizeof(Block);
  }

  // Return NULL if everything fails
  pthread_mutex_unlock(&mutex);
  return NULL;
}

/**
 * Find the level of the given size, so that we can search for a free block
 * in the free list.
 * @param  size size for which level is to be calculated
 * @return      level for the given size. This can be greater than MAX_INDEX,
 *              but can't be lesser than MIN_INDEX
 */
int find_level(size_t size) {
  // start at the smallest order available
  int level = 0;
  int level_mem = (int) pow(2, MIN_ORDER); // Memory at initial level

  // Keep increasing the level and the memory at that level
  // until we can satisfy `size`
  while (level_mem < size) {
    level_mem *= 2;
    level++;
  }
  return level;
}

/**
 * Prints malloc stats
 */
void malloc_stats() {
  printf("\t----------- Malloc stats -----------\n");
  printf("\tAllocated : %d\n", stats.uordblks);
  printf("\tNumber of Arenas: %d\n", stats.numArenas);
  printf("\tNumber of mmapped regions: %d\n", stats.hblks);
  printf("\tSpace allocated in mmapped regions (bytes): %d\n", stats.hblkhd);
  printf("\tTotal Allocation Requests: %d\n", stats.allocreq);
  printf("\tTotal Free Requests: %d\n", stats.allocreq);
  printf("\t------------------------------------\n");
}