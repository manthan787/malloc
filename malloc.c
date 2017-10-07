#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>


#define ERROR(e) \
  {              \
    perror(e);   \
    exit(-1);    \
  }

// Enum to assign a status to the memory blocks
typedef enum block_status_t {USED, FREE} BlockStatus;

// Memory Block to store metdata about the allocated memory blocks
typedef struct block_t {
	size_t size;
	struct block_t* next;
	struct block_t* previous;
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
static Block* blocks[10];

/**
 * Initialize the malloc library by extending heap size
 */
void init() {
	// Increment the data segment size by page size
	if((heap_break = sbrk(sysconf(_SC_PAGESIZE))) == (void *) -1) ERROR("sbrk");
	blocks[9] = (Block *) heap_break;
	blocks[9]->size = 4096;
	blocks[9]->next = NULL;
	blocks[9]->previous = NULL;
	blocks[9]->status = FREE;
}

int my_malloc(size_t size) {
	// Initialize if heap_break doesn't point to anything
	if(!heap) init();
}

int main() {
	init();
	printf("Initialized Heap!\n");
	return 0;
}