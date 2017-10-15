#include "realloc.h"
#include "free.h"
#include "malloc.h"
#include "block.h"
#include "config.h"
#include <string.h>
#include <math.h>
#include <stdio.h>

void *my_realloc(void *ptr, size_t size) {
  if (size == 0) {
    my_free(ptr);
    return NULL;
  }
  Block *new = my_malloc(size);
  Block *old = (Block *) (ptr - sizeof(Block));
  size_t old_size = pow(2, old->level + MIN_ORDER);
  memcpy(new, ptr, old_size - sizeof(Block));
  my_free(ptr);
  return new;
}