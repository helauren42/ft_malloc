#include "ft_malloc.h"
#include <stddef.h>

inline enum HEAP_TYPE getHeapType(const size_t bytesNeeded) {
  if (bytesNeeded <= TINY_MAX_PAYLOAD)
    return TINY;
  else if (bytesNeeded < SMALL_MAX_PAYLOAD)
    return SMALL;
  return LARGE;
}

inline t_zone *getHeapStart(const enum HEAP_TYPE heap_type) {
  switch (heap_type) {
  case TINY:
    return g_heaps.tiny_first;
  case SMALL:
    return g_heaps.small_first;
  case LARGE:
    return g_heaps.large_first;
  }
  return NULL;
}

inline void *getPayloadAddr(t_block *block) {
  return (void *)block + T_BLOCK_SIZE;
}

inline t_block *getHeaderAddr(void *payload) {
  // TODO check metadata
  return (void *)payload - T_BLOCK_SIZE;
}
