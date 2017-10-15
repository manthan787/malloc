#include "realloc.h"
#include "free.h"
#include "malloc.h"
#include "block.h"
#include "config.h"
#include "freelist.h"
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>

void *realloc(void *ptr, size_t size) {
  if (ptr == NULL) return NULL;
  if (size == 0) {
    free(ptr);
    return NULL;
  }
  Block *new = malloc(size);
  Block *old = (Block *) (ptr - sizeof(Block));
  size_t old_size = pow(2, old->level + MIN_ORDER);
  memcpy(new, ptr, old_size - sizeof(Block));
  free(ptr);
  return new;
}