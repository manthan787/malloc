#ifndef BLOCK_H
#define BLOCK_H
#include <stdlib.h>
// Enum to assign a status to the memory blocks
typedef enum block_status_t { USED, FREE } BlockStatus;

// Memory Block to store metdata about the allocated memory blocks
typedef struct block_t {
  void *startAddr;           // Starting address of the block
  size_t size;               // Size occupied at this block
  struct block_t *next;      // Next block
  struct block_t *previous;  // Previous Block
  BlockStatus status;        // If the block is occupied or free
} Block;

/**
 * Given an address, this function should create a new block
 * at that address and initialize its fields.
 *
 * Initial values are:
 * `startAddr`: is the specified address to this function.
 * `size`: 0, since this block hasn't been returned as a memory
 *        allocation request, yet.
 * `next`: NULL
 * `previous`: NULL
 * `status`: FREE
 */
Block *new_block(void *);

/**
 * Marks the given block as allocated
 */
Block *mark_block(Block *, size_t);

/**
 * Print a block in human-readable format
 */
void print_block(Block *, int);

#endif