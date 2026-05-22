#include "ft_malloc.h"
#include <stddef.h>

inline enum HEAP_TYPE getHeapType(const size_t bytesNeeded) {
  if (bytesNeeded <= TINY_MAX_PAYLOAD)
    return TINY;
  else if (bytesNeeded < SMALL_MAX_PAYLOAD)
    return SMALL;
  return LARGE;
}

inline t_heap *getHeapStart(const enum HEAP_TYPE heap_type) {
  switch (heap_type) {
  case TINY:
    return g_global.tiny_first;
  case SMALL:
    return g_global.small_first;
  case LARGE:
    return g_global.large_first;
  }
  return NULL;
}

inline void *getPayloadAddr(t_chunk *chunk) {
  return (void *)chunk + T_CHUNK_SIZE;
}

inline t_chunk *getHeaderAddr(void *payload) {
  // TODO check metadata
  return (void *)payload - T_CHUNK_SIZE;
}
