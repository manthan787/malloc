#ifndef BLOCK_H
#define BLOCK_H

// Enum to assign a status to the memory blocks
typedef enum block_status_t { USED, FREE } BlockStatus;

// Memory Block to store metdata about the allocated memory blocks
typedef struct block_t {
  void *startAddr; // Starting address of the block
  size_t size; // Size occupied at this block
  struct block_t *next; // Next block
  struct block_t *previous; // Previous Block
  BlockStatus status; // If the block is occupied or free
} Block;

// Creates a new block instance if given an address
Block* new_block(void*);

#endif