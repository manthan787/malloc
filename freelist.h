#ifndef FREELIST_H
#define FREELIST_H

// For `Block` type
#include "block.h"

/**
 * Allowed memory blocks in this buddy allocation implementation
 * The biggest memory block allowed is 4096 bytes and the smallest
 * block allowed is 8 bytes.
 * blocks[0] stores blocks having size 8 bytes
 * and blocks[9] stores blocks having size 4096 bytes
 */

 //Prints an entire block list in human readable format
void print_blocklist();

// Prints total free memory in the Block list in bytes
void free_memory(Block**, int);

// Partitions free lists to satisfy a memory request
void partition_blocks(Block**, int);
#endif