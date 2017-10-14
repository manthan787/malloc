#ifndef BLOCK_H
#define BLOCK_H
#include <stdlib.h>
// Enum to assign a status to the memory blocks
typedef enum block_status_t { USED, FREE } BlockStatus;

// Memory Block to store metdata about the allocated memory blocks
typedef struct block_t {
  struct block_t *next;      // Next block
  struct block_t *previous;  // Previous Block
  BlockStatus status;        // If the block is occupied or free
  short level;
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
 * `level`: Provided level
 */
Block *new_block(void *, int);

/**
 * Marks the given block as allocated
 */
Block *mark_block(Block *);

/**
 * Marks the given block as free
 */
void unmark_block(Block *);

/**
 * Print a block in human-readable format
 */
void print_block(Block *);

#endif