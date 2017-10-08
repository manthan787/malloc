#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "freelist.h"
#include "block.h"
#include "config.h"

void print_blocklist(Block** blocks, int length) {
  assert(length > 0);
  int i;
  for (i = 0; i < length; i++) {
    printf("Level %d\n", i);
    Block* b = blocks[i];
    while (b != NULL) {
      print_block(b);
      b = b->next;
    }
  }
}

/**
 * Partition blocks until memory request for given `order` can
 * be satisfied.
 * For example, if a memory request for order 8 (2048 bytes) is requested
 * a single order 9 block (4096 bytes) is partitioned.
 */
void partition_blocks(Block** blocks, int order) {
  // Start at MAX_INDEX as the split_idx
  int split_idx = MAX_INDEX + 1;
  printf("Start split at %d\n", split_idx);
  while(blocks[order] == NULL && split_idx > MIN_INDEX) {
    split_idx--;

    // If there are no blocks at given level, continue
    if(blocks[split_idx] == NULL) continue;

    // Otherwise, split the current index into two
    // Naturally, the new split index is the (split_idx - 1)
    int newsplit_idx = split_idx - 1;
    assert(blocks[newsplit_idx] == NULL);

    // Point to the start address of the block to be split, so that we can
    // update the head of the free list for `split_idx`
    void *addr = blocks[split_idx]->startAddr;
    blocks[split_idx] = blocks[split_idx]->next;

    // Create two new blocks at the address of previous bigger block, by
    // dividing it into two blocks, make them neighbors
    Block *block1 = new_block(addr, newsplit_idx);
    Block *block2 = new_block(addr + (int)pow(2, newsplit_idx + MIN_ORDER), newsplit_idx);
    block1->next = block2->startAddr;
    block2->previous = block1->startAddr;

    // Update the previous pointer of the new block assigned to the previous level
    // i.e. (split_idx)
    if(blocks[split_idx] != NULL) blocks[split_idx]->previous = block2->startAddr;

    // Put the first new block as the head of the free list for the newsplit_idx
    blocks[newsplit_idx] = block1;
  }
}