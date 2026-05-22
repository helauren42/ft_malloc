#include "ft_malloc.h"
#include <stdio.h>

void *ft_malloc(size_t size) {
  g_global.function_called = MALLOC;
  t_chunk *chunk = allocBlock(size);
  if (!chunk)
    return debugInfo("chunk not found"), NULL;
  return getPayloadAddr(chunk);
}
