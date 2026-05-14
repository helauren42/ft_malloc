#include "ft_malloc.h"
#include <stddef.h>

inline enum HEAP_TYPE getHeapType(const size_t bytesNeeded) {
  if (bytesNeeded <= TINY_MAX_BYTES)
    return TINY;
  else if (bytesNeeded < SMALL_MAX_BYTES)
    return SMALL;
  return LARGE;
}

inline void *getAddr(t_block *block) { return (void *)block + T_BLOCK_SIZE; }
