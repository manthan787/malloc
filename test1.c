#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "malloc.h"
#include "freelist.h"

int main() {
  printf("Allocated memory at %p\n", my_malloc(900));
  // printf("Allocated memory at %p\n", my_malloc(1000));
  // printf("Allocated memory at %p\n", my_malloc(900));
  // printf("Allocated memory at %p\n", my_malloc(200));
  print_blocklist(blocks, 10);
  return 0;
}