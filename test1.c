#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "malloc.h"
#include "free.h"
#include "block.h"

int main() {
  void *p4 = malloc(107384);
  print_block(p4 - sizeof(Block));
  print_blocklist();
  void *p = calloc(4, 10000);
  print_block(p - sizeof(Block));
  free(p);
  free(p4);
  return 0;
}