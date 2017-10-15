#include "calloc.h"
#include <stdio.h>
#include "malloc.h"
#include "freelist.h"
#include "block.h"
#include <string.h>
#include <pthread.h>
#include <sys/types.h>

void *calloc(size_t nmemb, size_t size) {
  size_t totalSize = nmemb * size;
  void *p = malloc(totalSize);
  if(p == NULL) {
  	return NULL;
  }
  pthread_mutex_lock(&mutex);
  memset(p, 0, totalSize);
  pthread_mutex_unlock(&mutex);
  return p;
}