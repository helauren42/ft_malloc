#include "ft_malloc.h"
#include <stdio.h>

void *ft_malloc(size_t size) {
  debugInfo("Called malloc");
  global.function_called = MALLOC;
  t_block *block = getBlock(size);
  if (!block)
    return debugInfo("block not found"), NULL;
  return getAddr(block);
}
