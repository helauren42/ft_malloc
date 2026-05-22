#include "ft_malloc.h"
#include <stdbool.h>
#include <stdio.h>
#include <sys/mman.h>

t_arenas g_global = {NULL, NULL, NULL, NULL, NULL, NULL, 0};

inline static t_heap **getLastZone(const enum HEAP_TYPE heap_type) {
  switch (heap_type) {
  case TINY:
    return &g_global.tiny_last;
  case SMALL:
    return &g_global.small_last;
  case LARGE:
    return &g_global.large_last;
  default:
    debugError("getFirstZone default case");
    return NULL;
  }
}

inline t_heap **getFirstZone(const enum HEAP_TYPE heap_type) {
  switch (heap_type) {
  case TINY:
    return &g_global.tiny_first;
  case SMALL:
    return &g_global.small_first;
  case LARGE:
    return &g_global.large_first;
  default:
    debugError("getFirstZone default case");
    return NULL;
  }
}

inline static t_heap *newLimitedZone(const enum HEAP_TYPE heap_type) {
  size_t heap_size = heap_type == TINY ? TINY_HEAP_SIZE : SMALL_HEAP_SIZE;
  t_heap *new_heap = NULL;
  new_heap = mmap(NULL, heap_size, MMAP_PROT, MMAP_FLAGS, -1, 0);
  if (new_heap == MAP_FAILED)
    return MAP_FAILED;
  return new_heap;
}

inline static void initNewZone(t_heap *new_heap,
                               const enum HEAP_TYPE heap_type) {
  // new heap
  new_heap->heap_type = heap_type;
  new_heap->next = NULL;
  new_heap->prev = NULL; // prev is set inside appendNewZone
  new_heap->active_chunk_count = 0;
  new_heap->first_free_chunk = (void *)(new_heap + 1);
  // first_free_chunk
  t_free_chunk *first_free_chunk = new_heap->first_free_chunk;
  first_free_chunk->prev = NULL;
  first_free_chunk->next = NULL;
  // both
  switch (heap_type) {
  case TINY:
    new_heap->free_bytes = TINY_HEAP_SIZE - T_HEAP_SIZE - T_CHUNK_SIZE;
    first_free_chunk->payload_bytes = TINY_HEAP_SIZE - T_CHUNK_SIZE;
    break;
  case SMALL:
    new_heap->free_bytes = SMALL_HEAP_SIZE - T_HEAP_SIZE - T_CHUNK_SIZE;
    first_free_chunk->payload_bytes = SMALL_HEAP_SIZE - T_CHUNK_SIZE;
    break;
  case LARGE:
    new_heap->free_bytes = 0;            // TODO
    first_free_chunk->payload_bytes = 0; // TODO
    break;
  default:
    debugError("errorHeapMetadataCorruption default case");
  }
}

inline static void appendNewZone(t_heap *new_heap,
                                 const enum HEAP_TYPE heap_type) {
  t_heap **last_heap;
  t_heap **first_heap;
  switch (heap_type) {
  case TINY:
    last_heap = &g_global.tiny_last;
    first_heap = &g_global.tiny_first;
    break;
  case SMALL:
    last_heap = &g_global.small_last;
    first_heap = &g_global.small_first;
    break;
  case LARGE:
    last_heap = &g_global.large_last;
    first_heap = &g_global.large_first;
    break;
  }
  if (*last_heap) {
    (*last_heap)->next = new_heap;
    new_heap->prev = *last_heap;
  } else {
    *first_heap = new_heap;
  }
  *last_heap = new_heap;
}

inline t_heap *newZone(const size_t size) {
  const enum HEAP_TYPE heap_type = getHeapType(size);
  // creating new heap
  t_heap *new_heap;
  if (heap_type == LARGE) {
    new_heap = mmap(NULL, size, MMAP_PROT, MMAP_FLAGS, -1, 0);
  } else {
    new_heap = newLimitedZone(heap_type);
  }
  if (new_heap == MAP_FAILED)
    return debugError("map failed bruhh"), NULL;
  // appending new heap to list of heaps
  initNewZone(new_heap, heap_type);
  appendNewZone(new_heap, heap_type);
  return new_heap;
}

inline void removeZone(t_chunk *chunk, t_heap *heap) {
  t_heap *prev = heap->prev;
  t_heap *next = heap->next;
  if (prev) {
    prev->next = next;
  } else {
    t_heap **heap = getFirstZone(getHeapType(chunk->payload_bytes));
    if (next)
      (*heap) = next;
    else
      (*heap) = NULL;
  }
  munmap(heap, T_HEAP_SIZE);
}
