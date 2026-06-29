#include "malloc.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void *malloc(size_t size) {
  pthread_mutex_lock(&g_global.mutex);
  g_global.function_called = MALLOC_FUNC;
  t_chunk *chunk = allocChunk(size);
  pthread_mutex_unlock(&g_global.mutex);
  if (!chunk)
    return debugInfo("chunk not found"), NULL;
  return getPayloadAddr(chunk);
}
