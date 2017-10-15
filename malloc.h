#ifndef MALLOC_H
#define MALLOC_H

#include <stdlib.h> // For size_t
#include "config.h"
#include "block.h"

void* malloc(size_t);

/** print malloc's stats for each arena */
void malloc_stats();

struct mallinfo {
   int numArenas; /* Total number of arenas */
   int allocreq;  /* Total number of allocation requests */
   int freereq;   /* Total number of free requests */
   int hblks;     /* Number of mmapped regions */
   int hblkhd;    /* Space allocated in mmapped regions (bytes) */
   int ordblks;   /* Number of free chunks */
   int uordblks;  /* Total allocated space (bytes) */
   int fordblks;  /* Total free space (bytes) */
};

struct mallinfo stats;
static __thread void *heap = NULL;
static __thread Block *blocks[MAX_INDEX + 1];
static __thread pthread_mutex_t mutex;
#endif