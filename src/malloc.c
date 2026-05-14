#include "ft_malloc.h"

void *malloc(size_t size) {
  write(STDOUT_FILENO, "Called malloc", ft_strlen("Called malloc"));
  global.function_called = MALLOC;
  t_block *block = getBlock(size);
  if (!block)
    return NULL;
  return getAddr(block);
}
