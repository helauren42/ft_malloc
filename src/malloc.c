#include "ft_malloc.h"
#include <stdio.h>

void *ft_malloc(size_t size) {
  g_heaps.function_called = MALLOC;
  t_block *block = allocBlock(size);
  if (!block)
    return debugInfo("block not found"), NULL;
  return getPayloadAddr(block);
}
